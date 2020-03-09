
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

#define LED_1 D0
#define LED_2 D4
#define BTN D8

void reset();
void led_reset();
void led_setup_done();
void read_btn();

bool send_request = false;

/*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/ /*****/

void reset()
{
    Serial.println("An error has occoured, resetting the esp");
    led_reset();
    delay(5000);
    ESP.restart();
}

void led_setup_done()
{
    for (int i = 0; i < 5; i++)
    {

        digitalWrite(LED_1, HIGH);
        digitalWrite(LED_2, LOW);
        delay(100);
        digitalWrite(LED_1, LOW);
        digitalWrite(LED_2, HIGH);
        delay(100);
    }

    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
}

void led_reset()
{
    for (int i = 0; i < 20; i++)
    {
        digitalWrite(LED_1, HIGH);
        digitalWrite(LED_2, LOW);
        delay(100);
        digitalWrite(LED_1, LOW);
        digitalWrite(LED_2, HIGH);
        delay(100);
    }

    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
}

void read_btn()
{
    if (digitalRead(BTN) == HIGH)
    {
        send_request = true;
        digitalWrite(LED_2, HIGH);
    }
}