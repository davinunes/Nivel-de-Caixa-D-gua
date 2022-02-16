/* ESTE SKETCH É  BASEADO NO EXEMPLO DA LIBRARY esp8266 and esp32 OLED driver for SSD1306
 * FOI MODIFICADO EM 31/10/2019 POR LEOTRÔNICO http://www.youtube.com/c/LeoTronicoLaboratoriodeeletr%C3%B4nica
 * Bibliografia: https://www.youtube.com/watch?v=dD2BqAsN96c
 * PINOUT da placa utilizada: https://raw.githubusercontent.com/AchimPieters/esp32-homekit-camera/master/Images/ESP32-30PIN-DEVBOARD.png
*/

/* INICIALIZA O Wifi
 * e o cliente http
*/

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Eternia";
const char* password = "celestia";
unsigned long lastTime = 0;
unsigned long timerDelay = 120000; //120 segundos

//Variaveis para usar para enviar mensagem no telegram
String chaveTelegram = "5199663658:AAF4D8-KtthX87TGX6pYHBiLGTTZYPyU3Z8";
String chat = "467782812"; //-1001601389998

/* INICIALIZA O DISPLAY COM A LIB SSD1306Wire.h
 * SE UTILIZAR O DISPLAY SSD1306:
 * SSD1306Wire  display(0x3c, SDA, SCL);
 * SDA -> GPIO21
 * SCL -> GPIO22
*/

#include "SSD1306Wire.h"
SSD1306Wire  display(0x3c, 21, 22);


/* INICIALIZA O Sonar com a LIB Ultrasonic.h
 * Modelos HC-SR04 ou SEN136B5B
*/


#include <Ultrasonic.h>

int distancex;          // Distancia medida pelo sensor em CM
int distance;          // Distancia medida pelo sensor em CM
int minimo;             // Menor distancia já medida
int maximo;             // Maior distancia já medida
int alturaMaxima = 100; // Altura maxima da coluna de água
int progresso = 0;      // Calculo da % da barra de progresso
int Delay = 500;        // Delay entre atualizações no LCD e no Serial Console

#define TRIGGER_PIN   33 // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN1     25 // Arduino pin tied to echo pin on ping sensor.

Ultrasonic sonar1(TRIGGER_PIN, ECHO_PIN1, 40000UL);


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  wifi();
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Ligado");
  Serial.println();


  // INICIALIZA O DISPLAY
  display.init();
  display.flipScreenVertically(); // INVERTE O DISPLAY VERTICALMENTE
}
//========================================================================

void loop() {

  distance = sonar();
  Serial.print("Sonar -> ");
  Serial.println(distance);

  if(minimo == 0){
    minimo = distance;
    }
  if(minimo > distance){
    minimo = distance;
    }

    if(maximo == 0){
    maximo = distance;
    }
  if(maximo < distance){
    maximo = distance;
    }

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "DISTANCIA DA TAMPA");
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 14, String(distance));
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 14, String(minimo));
  display.drawString(64, 28, String(maximo));
  
  //display.drawString(0, 40, String(minimo));
  //display.drawString(60, 40, String(maximo));

  distance > alturaMaxima ? progresso = alturaMaxima : progresso = distance;
  progresso = 100 * progresso/400;
  progresso = 103-progresso;
  
  display.drawString(90, 19, String(progresso));
  
  display.drawProgressBar(0,40, 127, 22, progresso);
  display.display();



  delay(Delay);

  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status()== WL_CONNECTED){
      Serial.print("\nHORA DE TAREFAS DA WEB \n");
      String msg = "Distancia do Sensor: " + String(distance) + "cm";
      telegram(msg);
        lastTime = millis(); // Esta linha sempre no final BLOCO QUE CONTROLA O TIMER!

    }else{
    wifi();
    }
  }

}

void wifi(){ //conecta no wifi
    int i = 0;
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(i++ > 10){
      break;
      }
  }
  
  }
int sonar(){
  distancex =  sonar1.read();
  for (int i = 0; i <= 10; i++) {
    distance = sonar1.read();
      if(distance < distancex){
          distancex = distance;
        };
      delay(60);
    }
  return distancex;
  }
String wget (String url){
  HTTPClient http;

      String serverPath = url;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        String payload = http.getString();
        return payload;

      }
      else {
          return "erro";
      }
      http.end();
  }

  void telegram(String mensagem){
   // ColeSeuTokenAqui ColeIDdoGrupoAqui TestandoEnvio
     String url = "https://api.telegram.org/bot" + chaveTelegram + "/sendMessage?chat_id=" + chat + "&text=" + mensagem;
     wget(url);
    }
