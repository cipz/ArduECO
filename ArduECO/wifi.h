
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

#include <ESP8266WiFi.h>

bool connect_wifi(const char *ssid, const char *password);

/*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/

bool wifi_scan(String ssid)
{
    Serial.println("Scanning available networks...");
    byte numSsid = WiFi.scanNetworks();

    // print the list of networks seen:
    Serial.print("SSID List:");
    Serial.println(numSsid);

    if (numSsid == 0)
        return false;

    // print the network number and name for each network found:
    for (int thisNet = 0; thisNet < numSsid; thisNet++)
    {
        // Serial.println(WiFi.SSID(thisNet))
        if (WiFi.SSID(thisNet) == ssid)
            return true;
    }

    return false;
}

//RIVEDERE MODALITA DI CONNESSIONE
bool connect_wifi(const char *ssid, const char *password)
{
    if (!wifi_scan(ssid))
    {
        Serial.println("Network not found.");
        return false;
    }

    Serial.println("Setting up WiFi connection.");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
    } //while

    Serial.println("");
    Serial.println("WiFi connected successfully");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    return true;

} //connect_wifi
