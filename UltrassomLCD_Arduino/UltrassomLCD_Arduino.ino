#include <Arduino.h>
#include <U8g2lib.h>
#include <NewPing.h>                // Bliblioteca para ver a distancia dos sensores

#define TRIGGER_PIN   3 // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN      4 // Arduino pin tied to echo pin on ping sensor.
#define MAX_DISTANCE 75 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
unsigned int pingSpeed = 500; // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
unsigned long pingTimer;     // Holds the next ping time.


int coluna = 65;      // Altura da Coluna de água // 8cm distancia do sensor

U8G2_PCD8544_84X48_F_4W_SW_SPI tela(U8G2_R0, /* clock=*/ 11, /* data=*/ 10, /* cs=*/ 7, /* dc=*/ 9, /* reset=*/ 8);  // Nokia 5110 Display

void barraSinal(int largura=84, int maximo=50) // Exibe uma barra de Sinal baseado em um valor passado
{ 
  
  float tangente=0.46630;           // base de referencia 15°
  int x=0, y=44;
  largura > maximo ? largura = maximo : largura = largura;  // cateto adjacente = parametro informado
  largura = 80 * largura / maximo;
  int altura=largura*tangente;      // cateto oposto = deve ser dinamico
  int altura_y=y-altura;
  tela.drawTriangle(x,y,largura,y,largura,altura_y);
}

void echoCheck() { // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  // Don't do anything here!
  if (sonar.check_timer()) { // This is how you check to see if the ping was received.
    // Here's where you can add code.
    Serial.print("Ping: ");
    Serial.print(sonar.ping_result / US_ROUNDTRIP_CM); // Ping returned, uS result in ping_result, convert to cm with US_ROUNDTRIP_CM.
    Serial.println("cm");
  }
  // Don't do anything here!
}

void setup(void) {
  tela.begin();
  pingTimer = millis(); // Start now.
}

void loop(void) {

  if (millis() >= pingTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    pingTimer += pingSpeed;      // Set the next ping time.
    sonar.ping_timer(echoCheck); // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  }

  long cm = sonar.ping_result / US_ROUNDTRIP_CM;

  coluna = 60-cm;
  
  tela.clearBuffer();          // clear the internal memory
  tela.setFont(u8g2_font_ncenB14_tr); // choose a suitable font
  tela.drawRFrame(0,0,84,48,5);
  tela.setCursor(4, 20);
  tela.print(coluna);  // write something to the internal memory
  tela.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  tela.print("-");  // write something to the internal memory
  tela.print(cm);  // write something to the internal memory
  barraSinal(coluna);
  tela.sendBuffer();          // transfer internal memory to the display
  delay(100); 

  
 
}
