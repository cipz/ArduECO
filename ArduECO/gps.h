
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

#include <SoftwareSerial.h>
#include <TinyGPS.h>

// Creating the GPS object
SoftwareSerial gps_serial(4, 5); //RX, TX
TinyGPS gps;

// Declaring the GPS functions
String get_gps_position();
String gps_dump_string(TinyGPS &gps);
void gpsdump(TinyGPS &gps);
void printFloat(double f, int digits = 2);
String getFloat(double f, int digits = 2);

/*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/

String get_gps_position()
{
    String output;

    bool newdata = false;
    unsigned long start = millis();

    // Every 5 seconds we print an update
    while (millis() - start < 5000)
    {
        if (gps_serial.available())
        {
            char c = gps_serial.read();
            //Serial.print(c); // uncomment to see raw GPS data
            if (gps.encode(c))
            {
                Serial.println(gps.encode(c));
                Serial.println("GPS ENCODE OK");
                newdata = true;
                break; // uncomment to print new data immediately!
            }
        }
        yield();
    } //while

    if (newdata)
    {
        Serial.println("Acquired Data");
        Serial.println("-------------");
        // gpsdump(gps);
        output = gps_dump_string(gps);
        Serial.println("-------------");
        Serial.println();
    }
    else
    {
        Serial.println('.');
        output = '.';
    }

    return output;
}

String gps_dump_string(TinyGPS &gps)
{
    float flat, flon;
    unsigned long age, date, time, chars;
    int year;
    byte month, day, hour, minute, second, hundredths;

    gps.f_get_position(&flat, &flon, &age);

    String flat_s = getFloat(flat, 5);
    String flon_s = getFloat(flon, 5);

    gps.get_datetime(&date, &time, &age);

    String date_s = (String)date;
    String time_s = (String)time;

    if (date < 100000)
        date_s = "0" + date_s;

    String output = "\"lt\" : \"" + flat_s + "\", \
\"lg\" : \"" + flon_s +
                    "\", \
\"dt\" : \"" + date_s +
                    "." + time_s + "\", ";

    Serial.println("\n\n\n" + output);

    return output;

} //gps_dump

void gpsdump(TinyGPS &gps)
{
    long lat, lon;
    float flat, flon;
    unsigned long age, date, time, chars;
    int year;
    byte month, day, hour, minute, second, hundredths;
    unsigned short sentences, failed;

    gps.get_position(&lat, &lon, &age);
    Serial.print("Lat/Long(10^-5 deg): ");
    Serial.print(lat);
    Serial.print(", ");
    Serial.print(lon);
    Serial.print(" Fix age: ");
    Serial.print(age);
    Serial.println("ms.");

    // On Arduino, GPS characters may be lost during lengthy Serial.print()
    // On Teensy, Serial prints to USB, which has large output buffering and
    //   runs very fast, so it's not necessary to worry about missing 4800
    //   baud GPS characters.

    gps.f_get_position(&flat, &flon, &age);
    Serial.print("Lat/Long(float): ");
    printFloat(flat, 5);
    Serial.print(", ");
    printFloat(flon, 5);
    Serial.print(" Fix age: ");
    Serial.print(age);
    Serial.println("ms.");

    gps.get_datetime(&date, &time, &age);
    Serial.print("Date(ddmmyy): ");
    Serial.print(date);
    Serial.print(" Time(hhmmsscc): ");
    Serial.print(time);
    Serial.print(" Fix age: ");
    Serial.print(age);
    Serial.println("ms.");

    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
    Serial.print("Date: ");
    Serial.print(static_cast<int>(month));
    Serial.print("/");
    Serial.print(static_cast<int>(day));
    Serial.print("/");
    Serial.print(year);
    Serial.print("  Time: ");
    Serial.print(static_cast<int>(hour + 8));
    Serial.print(":"); //Serial.print("UTC +08:00 Malaysia");
    Serial.print(static_cast<int>(minute));
    Serial.print(":");
    Serial.print(static_cast<int>(second));
    Serial.print(".");
    Serial.print(static_cast<int>(hundredths));
    Serial.print(" UTC +08:00 Malaysia");
    Serial.print("  Fix age: ");
    Serial.print(age);
    Serial.println("ms.");

    Serial.print("Alt(cm): ");
    Serial.print(gps.altitude());
    Serial.print(" Course(10^-2 deg): ");
    Serial.print(gps.course());
    Serial.print(" Speed(10^-2 knots): ");
    Serial.println(gps.speed());
    Serial.print("Alt(float): ");
    printFloat(gps.f_altitude());
    Serial.print(" Course(float): ");
    printFloat(gps.f_course());
    Serial.println();
    Serial.print("Speed(knots): ");
    printFloat(gps.f_speed_knots());
    Serial.print(" (mph): ");
    printFloat(gps.f_speed_mph());
    Serial.print(" (mps): ");
    printFloat(gps.f_speed_mps());
    Serial.print(" (kmph): ");
    printFloat(gps.f_speed_kmph());
    Serial.println();

    gps.stats(&chars, &sentences, &failed);
    Serial.print("Stats: characters: ");
    Serial.print(chars);
    Serial.print(" sentences: ");
    Serial.print(sentences);
    Serial.print(" failed checksum: ");
    Serial.println(failed);
}

void printFloat(double number, int digits)
{
    // Handle negative numbers
    if (number < 0.0)
    {
        Serial.print('-');
        number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for (uint8_t i = 0; i < digits; ++i)
        rounding /= 10.0;

    number += rounding;

    // Extract the integer part of the number and print it
    unsigned long int_part = (unsigned long)number;
    double remainder = number - (double)int_part;
    Serial.print(int_part);

    // Print the decimal point, but only if there are digits beyond
    if (digits > 0)
        Serial.print(".");

    // Extract digits from the remainder one at a time
    while (digits-- > 0)
    {
        remainder *= 10.0;
        int toPrint = int(remainder);
        Serial.print(toPrint);
        remainder -= toPrint;
    }
}

String getFloat(double number, int digits)
{

    String floatval = "";

    // Handle negative numbers
    if (number < 0.0)
    {
        floatval += '-';
        number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for (uint8_t i = 0; i < digits; ++i)
        rounding /= 10.0;

    number += rounding;

    // Extract the integer part of the number and print it
    unsigned long int_part = (unsigned long)number;
    double remainder = number - (double)int_part;
    floatval += int_part;

    // Print the decimal point, but only if there are digits beyond
    if (digits > 0)
        floatval += '.';

    // Extract digits from the remainder one at a time
    while (digits-- > 0)
    {
        remainder *= 10.0;
        int toPrint = int(remainder);
        floatval += toPrint;
        remainder -= toPrint;
    }

    return floatval;
}
