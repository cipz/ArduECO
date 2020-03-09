
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

#define MQTT_MAX_PACKET_SIZE 1024

#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

bool cert_setup();
void callback(char *topic, byte *payload, unsigned int length);
void reconnect();

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
WiFiClientSecure espClient;
const char *reconnect_message;

//MQTT broker ip
// Understand how to declare it after declaring the pubsub client
//const char *AWS_endpoint = "a3mnvbd65ag416-ats.iot.us-east-2.amazonaws.com";

int value = 0;
char msg[1000];

//set MQTT port number to 8883 as per standard
PubSubClient client(AWS_endpoint, 8883, callback, espClient);

/*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] : ");

    for (int i = 0; i < length; i++)
        Serial.print((char)payload[i]);

    Serial.println();
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");

        // Attempt to connect
        if (client.connect("ESPthing"))
        {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish(out_topic, reconnect_message);
            // ... and resubscribe
            client.subscribe(in_topic);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");

            char buf[256];
            espClient.getLastSSLError(buf, 256);
            Serial.print("WiFiClientSecure SSL error: ");
            Serial.println(buf);

            // Wait 5 seconds before retrying
            delay(5000);
        } //if

    } //while

} //reconnect

bool cert_setup()
{
    // Reading ROM
    if (!SPIFFS.begin())
    {
        Serial.println("Failed to mount file system");
        return false;
    } //if

    // Load certificate file
    File cert = SPIFFS.open("/cert.der", "r");
    if (!cert)
    {
        Serial.println("Failed to open cert file");
        return false;
    }
    Serial.println("Success to open cert file");

    delay(1000);

    if (!espClient.loadCertificate(cert))
    {
        Serial.println("cert not loaded");
        return false;
    }
    Serial.println("cert loaded");

    // Load private key file
    File private_key = SPIFFS.open("/private.der", "r"); //replace private eith your uploaded file name
    if (!private_key)
    {
        Serial.println("Failed to open private cert file");
        return false;
    }
    Serial.println("Success to open private cert file");

    delay(1000);

    if (!espClient.loadPrivateKey(private_key))
    {
        Serial.println("private key not loaded");
        return false;
    }
    Serial.println("private key loaded");

    // Load CA file
    File ca = SPIFFS.open("/ca.der", "r"); //replace ca eith your uploaded file name
    if (!ca)
    {
        Serial.println("Failed to open ca ");
        return false;
    }
    Serial.println("Success to open ca");

    delay(1000);

    if (!espClient.loadCACert(ca))
    {
        Serial.println("ca failed");
        return false;
    }
    Serial.println("ca loaded");

    Serial.print("Heap: ");
    Serial.println(ESP.getFreeHeap());

    return true;
}
