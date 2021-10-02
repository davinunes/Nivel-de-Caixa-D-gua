#include <AdafruitIO.h>
#include <AdafruitIO_Dashboard.h>
#include <AdafruitIO_Data.h>
#include <AdafruitIO_Definitions.h>
#include <AdafruitIO_Feed.h>
#include <AdafruitIO_Group.h>
#include <AdafruitIO_MQTT.h>
#include <AdafruitIO_Time.h>
#include <AdafruitIO_WiFi.h>

#include <Ultrasonic.h>
#include "config.h"

// https://www.studiopieters.nl/esp32-pinout/

int distance1;
int distance2;
int Delay = 10000;

#define ECHO_PIN2     33 // D15 - Arduino pin tied to trigger pin on ping sensor.
#define TRIGGER_PIN   26 // D15 - Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN1     25 // D2  - Arduino pin tied to echo pin on ping sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

Ultrasonic sonar1(TRIGGER_PIN, ECHO_PIN1); // NewPing setup of pins and maximum distance.
Ultrasonic sonar2(TRIGGER_PIN, ECHO_PIN2); // NewPing setup of pins and maximum distance.
 
// set up the 'counter' feed
AdafruitIO_Feed *sonar01 = io.feed("sonar01");
AdafruitIO_Feed *sonar02 = io.feed("sonar02");
AdafruitIO_Feed *range01 = io.feed("range01");

void setup() {
  Serial.begin(115200);
  
  io.connect();

  // Quando receber um update da adafruit, executa a função Message
  range01->onMessage(Message);
  range01->get();

  
// wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  // we are connected
  Serial.println("Estamos conectados: ");
  Serial.println(io.statusText());

}

void loop() {



    //Serial.println(str);

    distance1 = sonar1.read();
    distance2 = sonar2.read();
    
    io.run();
    Serial.print("publicando sonar1 -> ");
    Serial.println(distance1);

    Serial.print("publicando sonar2 -> ");
    Serial.println(distance2);
    
    sonar01->save(distance1);
    sonar02->save(distance2);


    delay(Delay);
 }



void Message(AdafruitIO_Data *data) {
  String tmp = data->value();
  Serial.print("RECEBIDO <- ");
  Serial.println(tmp);
  Delay = tmp.toInt();
  Delay *= 1000;
  Serial.print("Novo timeout <- ");
  Serial.println(Delay);

}

void reverse(){
int i = 0;
int j = strlen(str) - 1;

    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}
