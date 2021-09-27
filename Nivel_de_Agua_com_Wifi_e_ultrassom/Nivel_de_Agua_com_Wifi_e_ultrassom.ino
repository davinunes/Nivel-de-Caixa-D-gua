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

int distance;

#define TRIGGER_PIN   26 // D15 - Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN      25 // D2  - Arduino pin tied to echo pin on ping sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

Ultrasonic sonar(TRIGGER_PIN, ECHO_PIN); // NewPing setup of pins and maximum distance.
 
// set up the 'counter' feed
AdafruitIO_Feed *ultrassom01 = io.feed("ultrassom01");

void setup() {
  Serial.begin(115200);
  io.connect();
// wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

    distance = sonar.read();
    
    io.run();
    Serial.print("sending -> ");
    Serial.println(distance);

    ultrassom01->save(distance);


    delay(3000);
 }
