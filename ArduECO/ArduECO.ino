
/**
 * 
 *   File   :
 *   Author : Ciprian Voinea
 *   Date   : 02/2020
 *   Course : Wireless Networks
 *   Desc   :
 *   Usage  :
 * 
 */

#include "utils.h"

#define MIN_INT -32, 768
#define MAX_INT 32, 767

#include <ArduinoJson.h>
StaticJsonBuffer<500> jsonBuffer;

#include "FS.h"

#include "sd.h"
#include "wifi.h"
#include "aws.h"
#include "gps.h"

#include "MQ7.h"
MQ7 mq7(A0, 3.3);

// 60 sec = 60000 millis
#define LOOP_DELAY 1000 * 5

File cache_log_file;
File perm_log_file;

bool time_client = false;

unsigned long start;

/*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/

void setup()
{
    Serial.begin(9600);
    Serial.setDebugOutput(true);

    Serial.println("\n\nInitializing boot sequence");

    Serial.println("Setting pinMode() s");

    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    pinMode(BTN, INPUT);

    Serial.println("Checking certificates");
    if (!cert_setup())
        reset();
    Serial.println("Certificates correctly loaded!");

    Serial.print("Initializing SD card");
    while (!SD.begin(CS_PIN))
    {
        Serial.println("SD card Initialization failed!\nRetrying in 5 seconds...");
        delay(5000);
    } //while
    Serial.print("Serial card correctly initialized!");

    Serial.println("\nContent of SD card\n");
    File root = SD.open("/");
    printFiles(root, 0);

    Serial.println("Acquiring parameters");
    if (!get_params())
        reset();
    Serial.println("All parameters correctly acquired!");

    client.setServer(AWS_endpoint, 8883);

    Serial.println("Initializing gps unit");
    gps_serial.begin(9600);

    Serial.println("Creating log files");

    // Remove the cache_log file to crete it from scratch
    if (SD.exists("cache_log.txt"))
        SD.remove("cache_log.txt");

    // Creating files
    cache_log_file = SD.open("cache_log.txt", FILE_WRITE);
    perm_log_file = SD.open("perm_log.txt", FILE_WRITE);

    cache_log_file.close();
    perm_log_file.close();

    Serial.println("Log files correctly created!");

    Serial.print("Sizeof(gpsobject) = ");
    Serial.println(sizeof(TinyGPS));

    Serial.print("Generating current_id for the ride: ");
    current_id = random(MAX_INT);
    Serial.println(current_id);

    Serial.println("\n\nBoot sequence finished correctly!\n\n");

    led_setup_done();

    start = millis();

} //setup

/*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/

void loop()
{
    read_btn();

    if (send_request)
    {
        Serial.println("Send button has been pressed");
        digitalWrite(LED_2, LOW);

        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("Connection attempt: ");
            if (!connect_wifi(ssid, password))
                Serial.print("UN");
            Serial.println("SUCCESSFULL");
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            // https://raphberube.com/blog/2019/02/18/Making-the-ESP8266-work-with-AWS-IoT.html
            Serial.println("Client connected to wifi");

            if (!time_client)
            {
                timeClient.begin();
                while (!timeClient.update())
                    timeClient.forceUpdate();
                espClient.setX509Time(timeClient.getEpochTime());
                time_client = true;
            }

            if (client.connected())
            {
                Serial.println("Sending all the contents of the cache file");

                client.loop();

                long now = millis();

                int stringIndex = 0;
                char inputString[50];
                int inputChar;

                cache_log_file = SD.open("cache_log.txt");
                inputChar = cache_log_file.read();

                while (inputChar != -1)
                {

                    digitalWrite(LED_2, HIGH);

                    if (inputChar != '\n')
                    {
                        inputString[stringIndex] = inputChar;
                        stringIndex++;
                        inputString[stringIndex] = '\0';
                    }
                    else
                    {
                        Serial.print("[Sending] : ");
                        Serial.println(inputString);
                        client.publish(out_topic, inputString);
                        digitalWrite(LED_2, LOW);
                        delay(100);
                        stringIndex = 0;
                    }
                    inputChar = cache_log_file.read();
                }

                Serial.println("All records have been sent to AWS");

                cache_log_file.close();

                SD.remove("cache_log.txt");
                cache_log_file = SD.open("cache_log.txt", FILE_WRITE);
                cache_log_file.close();

                send_request = false;
            }
            else
            {
                int inputChar;

                cache_log_file = SD.open("cache_log.txt");
                inputChar = cache_log_file.read();

                int msg_count = 0;

                while (inputChar != -1)
                {
                    if (inputChar == '\n')
                        msg_count++;

                    inputChar = cache_log_file.read();
                }

                cache_log_file.close();

                String reconnect_message_tmp = "{\
\"id\" : \"" + (String)current_id + "\", \
\"tpc\" : \"" + out_topic + "\", \
\"msg\" : \"" + (String)msg_count + "\" \
}";

                reconnect_message = reconnect_message_tmp.c_str();

                reconnect();
            }
        }
    }

    read_btn();

    if (wifi_scan(ssid))
        digitalWrite(LED_1, HIGH);
    else
        digitalWrite(LED_1, LOW);

    read_btn();

    if (millis() - start > LOOP_DELAY)
    {
        String gps = get_gps_position();

        if (gps != (String)'.')
        {
            float ppm = mq7.getPPM();
            Serial.println(ppm);

            // Creating string to save in file
            String current_message = "{\
\"id\" : \"" + (String)current_id + "\", \
\"air\" : \"" + (String)ppm + "\", \
" + gps + " \
\"tpc\" : \"" + out_topic + "\"\
}";

            Serial.println("Saving to sd card... ");
            Serial.println(current_message);

            cache_log_file = SD.open("cache_log.txt", FILE_WRITE);
            perm_log_file = SD.open("perm_log.txt", FILE_WRITE);

            cache_log_file.println(current_message);
            perm_log_file.println(current_message);

            cache_log_file.close();
            perm_log_file.close();
        }

        Serial.println("\n\n\nCurrent READ ended, waiting for another\n\n\n");

        start = millis();
    }
    
    read_btn();

    delay(20);

} //loop
