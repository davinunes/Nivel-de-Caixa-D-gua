#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
Adafruit_SSD1306 display(-1);

#define TRIGGER_PIN  4 // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN1    3 // Arduino pin tied to echo pin on ping sensor.
#include <Ultrasonic.h>
Ultrasonic sonar1(TRIGGER_PIN, ECHO_PIN1, 40000UL);


int prog = 100; // de 8 a 124 (7 + x entre 1 e 117)
// the setup function runs once when you press reset or power the board

int distance = 0;
int _distance = 0;

void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
}

// the loop function runs over and over again forever
void loop() {
  sonar();
  tela(distance);
  delay(50);
}


void tela(int nivel){
  int _nivel = nivel*117/100;
  int barra = _nivel +7;
   // Limpa o display
  display.clearDisplay();
  display.setFont();
  // Texto no display
  display.setTextSize(1); // Fonte do texto
  display.setTextColor(WHITE); // Cor do texto
  display.setCursor(0,0); // Posiciona o cursor
  display.println("Cheio: "+String(nivel)+"% :: "+_distance+"cm"); // Imprime mensagem
  //drawRect(x, y, width, height,ROUND, color)
  display.drawRoundRect(0, 8, 128, 24, 6, WHITE); // 20
  if(nivel > 0){
    display.fillRoundRect(2, 10, barra, 20, 6, WHITE); //16
  }
  display.display();
}
void sonar() {
  distance = sonar1.read();
  _distance = distance;
  distance = distance - 8;
  Serial.print("Sonar -> ");
  Serial.println(distance);

  distance > 58 ? distance = 58 : distance = distance;
  distance = 100 * distance / 58;
  distance = 100 - distance;

}
