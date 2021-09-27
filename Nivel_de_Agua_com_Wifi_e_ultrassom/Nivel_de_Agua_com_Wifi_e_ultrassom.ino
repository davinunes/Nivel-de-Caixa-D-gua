#include <Ultrasonic.h>
#include <WiFi.h>
#include <HTTPClient.h>

// https://www.studiopieters.nl/esp32-pinout/
const char* ssid = "Eternia";
const char* password = "celestia";

String setSensor = "https://a.b.c";

int distance;

#define TRIGGER_PIN   26 // D15 - Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN      25 // D2  - Arduino pin tied to echo pin on ping sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
Ultrasonic sonar(TRIGGER_PIN, ECHO_PIN); // NewPing setup of pins and maximum distance.


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  int i = 0;
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  //  if(i++ > 10){
  //    break;
  //    }
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  

}

void loop() {
  distance = sonar.read();
  Serial.println(distance);
  delay(500);
}
