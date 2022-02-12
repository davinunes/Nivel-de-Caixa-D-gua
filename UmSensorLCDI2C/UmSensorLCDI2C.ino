/* ESTE SKETCH É  BASEADO NO EXEMPLO DA LIBRARY esp8266 and esp32 OLED driver for SSD1306
 * FOI MODIFICADO EM 31/10/2019 POR LEOTRÔNICO http://www.youtube.com/c/LeoTronicoLaboratoriodeeletr%C3%B4nica
 * PARA FACILITAR A EXPLICAÇÃO SOBRE A CONEXÃO E FUNCIONAMENTO DO ESP32-WROOM E DISPLAY OLED
 * NESTA VERSÃO VOCE APENAS DESCOMENTA A LINHA QUE QUER VISUALIZAR OS EFEITOS
*/

#include "SSD1306Wire.h" // INCLUI: `#include "SSD1306.h"`

// INICIALIZA O DISPLAY COM A LIB SSD1306Wire.h

//SE UTILIZAR O DISPLAY SSD1306:
//SSD1306Wire  display(0x3c, SDA, SCL);
//SDA -> GPIO21
//SCL -> GPIO22
//https://raw.githubusercontent.com/AchimPieters/esp32-homekit-camera/master/Images/ESP32-30PIN-DEVBOARD.png
//https://www.youtube.com/watch?v=dD2BqAsN96c
SSD1306Wire  display(0x3c, 21, 22);
//SE UTILIZAR O DISPLAY SH1106:
// SH1106 display(0x3c, SDA, SCL);


// Sensor de Ultrassom
#include <Ultrasonic.h>
int distance1;
int minimo;
int maximo;
int alturaMaxima = 100;
int progresso = 0;
int Delay = 200;

#define TRIGGER_PIN   33 // D15 - Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN1     25 // D2  - Arduino pin tied to echo pin on ping sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

Ultrasonic sonar1(TRIGGER_PIN, ECHO_PIN1, 40000UL); // NewPing setup of pins and maximum distance.


void setup() {
  Serial.begin(115200);
  Serial.println("Ligado");
  Serial.println();


  // INICIALIZA O DISPLAY
  display.init();
  display.flipScreenVertically(); // INVERTE O DISPLAY VERTICALMENTE
}
//========================================================================
void drawFontFaceDemo() {
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    display.clear();   
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "OI MUNDO");
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 14, "OI MUNDO");
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 32, "OI MUNDO");
    display.display();  
       
}





void loop() {

  distance1 = sonar1.read();
  Serial.print("publicando sonar1 -> ");
  Serial.println(distance1);

  if(minimo == 0){
    minimo = distance1;
    }
  if(minimo > distance1){
    minimo = distance1;
    }

    if(maximo == 0){
    maximo = distance1;
    }
  if(maximo < distance1){
    maximo = distance1;
    }

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "DISTANCIA DA TAMPA");
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 14, String(distance1));
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 14, String(minimo));
  display.drawString(64, 28, String(maximo));
  
  //display.drawString(0, 40, String(minimo));
  //display.drawString(60, 40, String(maximo));

  distance1 > alturaMaxima ? progresso = alturaMaxima : progresso = distance1;
  progresso = 100 * progresso/400;
  progresso = 100-progresso;
  
  display.drawString(90, 19, String(progresso));
  
  display.drawProgressBar(0,40, 127, 22, progresso);
  display.display();



  delay(Delay);

    
}
