
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

#include <SPI.h>
#include <SD.h>

// CS pin for the micro SD card
#define CS_PIN 0

File params;
const char *ssid;
const char *password;
const char *in_topic;
const char *out_topic;
const char *AWS_endpoint;

bool get_params();
void printFiles(File dir, int numTabs);

unsigned long current_id;

/*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/

bool get_params()
{
    if (!SD.exists("params.json"))
        return false;

    params = SD.open("params.json");
    JsonObject &root = jsonBuffer.parseObject(params);
    params.close();

    ssid = root["ssid"];
    password = root["password"];
    in_topic = root["in_topic"];
    out_topic = root["out_topic"];
    AWS_endpoint = root["endpoint"];

    Serial.println(ssid);
    Serial.println(password);
    Serial.println(in_topic);
    Serial.println(out_topic);
    Serial.println(AWS_endpoint);

    return true;
}

void printFiles(File dir, int numTabs)
{
    while (true)
    {
        File entry = dir.openNextFile();
        if (!entry)
        {
            break;
        }
        for (uint8_t i = 0; i < numTabs; i++)
        {
            Serial.print('\t');
        }
        Serial.println(entry.name());
        entry.close();
    }
}
