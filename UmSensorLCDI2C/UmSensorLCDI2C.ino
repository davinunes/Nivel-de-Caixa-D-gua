/* ESTE SKETCH É  BASEADO NO EXEMPLO DA LIBRARY esp8266 and esp32 OLED driver for SSD1306
   FOI MODIFICADO EM 31/10/2019 POR LEOTRÔNICO http://www.youtube.com/c/LeoTronicoLaboratoriodeeletr%C3%B4nica
   Bibliografia: https://www.youtube.com/watch?v=dD2BqAsN96c, https://www.curtocircuito.com.br/blog/Categoria%20IoT/desenvolvimento-de-dashboard-mqtt-com-adafruitio
   PINOUT da placa utilizada: https://raw.githubusercontent.com/AchimPieters/esp32-homekit-camera/master/Images/ESP32-30PIN-DEVBOARD.png
*/

// CONFIGURAÇÕES DA INSTALAÇÃO

int idSensor = 99;    // id do sensor no site davinunes.eti.br (ou outro que for escolhido)
int distanciaSonda = 1 ; // A que distancia a sonda está do nivel máximo de água
int alturaAgua = 150; // Altura maxima da coluna de água (Não considerar a distancia da Sonda)
String nomeDaSonda = "Desenvolvimento Reservatório 01"; // Nome da Sonda nas mensagens do Telegram
const char* ssid = "Eternia"; // Nome da Rede Wifi  
const char* password = "celestia"; // Senha da rede Wifi

// Endereços do github que serão utilizados para ajustar variáveis remotas:
String IntervaloDePush = "https://github.com/davinunes/TopLifeMiami-Nivel-de-gua/blob/main/parametros/updateTime";
String nivelAlertaTelegram = "https://raw.githubusercontent.com/davinunes/TopLifeMiami-Nivel-de-gua/main/parametros/nivelAlerta";
String urlSite = "h2o-miami.davinunes.eti.br";

//Variaveis para usar para enviar mensagem no telegram
String chaveTelegram = "5199663658:AAF4D8-KtthX87TGX6pYHBiLGTTZYPyU3Z8";
String chat = "-1001158157448"; //Chat do telgram que receberá qualquer leitura do sensor
String alert = "-1001601389998"; // Chat do telgram que receberá mensagem apenas quando o nivel da água estiver abaixo do esperado

// Variaveis relacionadas a Adafruit
#define IO_USERNAME "ilunne" //usuario
#define IO_KEY2    "hMjk89XNaWhSBc7UxR70upfJch2A" //Cole apenas a parte depois do underline
#define FEED_CM "/feeds/PlacaDSV_Sonda01_centimetros"
#define FEED_PERC "/feeds/PlacaDSV_Sonda01_porcentagem"

/* INICIALIZA O Wifi
   e o cliente http
   Todas as variáveis abaixo são referentes ao wifi e ao delay entre os acessos a internet
*/

#include <WiFi.h>
#include <HTTPClient.h>

unsigned long lastTime = 0;
unsigned long lastTimeAlert = 0;
unsigned long timerDelay = 60000; //120 segundos
unsigned long timerAlerta = 300000; //5 minutos
unsigned long nivelAlerta = 80; //80%
WiFiClient client;
String StatusInternet = "Sem Wifi...";


/* INICIALIZA O DISPLAY COM A LIB SSD1306Wire.h
   SE UTILIZAR O DISPLAY SSD1306:
   SSD1306Wire  display(0x3c, SDA, SCL);
   SDA -> GPIO21
   SCL -> GPIO22
*/

#include "SSD1306Wire.h"
SSD1306Wire display(0x3c, 21, 22);


/* Biblioteca Adafruit

*/

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define IO_SERVER     "io.adafruit.com"
#define IO_SERVERPORT 1883
#define IO_KEY1    "aio_"
#define IO_USERNAME "ilunne"
#define IO_KEY2    "hMjk89XNaWhSBc7UxR70upfJch2A"

Adafruit_MQTT_Client mqtt(&client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_KEY1 IO_KEY2);
Adafruit_MQTT_Publish sonar01 = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME FEED_CM, MQTT_QOS_1);
Adafruit_MQTT_Publish sonar02 = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME FEED_PERC, MQTT_QOS_1);
Adafruit_MQTT_Subscribe wifiTime = Adafruit_MQTT_Subscribe(&mqtt, IO_USERNAME "/feeds/PlacaDSV_update_time");



/* INICIALIZA O Sonar com a LIB Ultrasonic.h
   Modelos HC-SR04 ou SEN136B5B
*/

#define TRIGGER_PIN  33 // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN1    25 // Arduino pin tied to echo pin on ping sensor.


#include <Ultrasonic.h>

Ultrasonic sonar1(TRIGGER_PIN, ECHO_PIN1, 40000UL);

int distancex;          // Distancia medida pelo sensor em CM
int distance;         // Distancia medida pelo sensor em CM
int minimo;            // Menor distancia já medida
int maximo;            // Maior distancia já medida
int progresso = 0;      // Calculo da % da barra de progresso


/*

   ===========================================
   Configuração durante o boot do ESP32
   ===========================================

*/

/*
 * 
 * Protótipos
 * 
 * das
 * 
 * funções
 */

String wget (String url);
void telegramLog(String mensagem);
void telegramAlarm(String mensagem);
void eti(int num);
void conectar_broker();
void callback();
void sonar();
void tela();
void IoT();
void internet();
void getParametrosRemotos();

/*
 * Setup
 */

void setup() {
  Serial.begin(115200);
  internet();

  // INICIALIZA O DISPLAY & INVERTE O DISPLAY VERTICALMENTE
  display.init();
  display.flipScreenVertically();


  // Realiza as inscrições MQTT
  mqtt.subscribe(&wifiTime);
}

//========================================================================

void loop() {
  sonar();
  tela();
  IoT();
}

/*
 * funções
 */


String wget (String url) {
  HTTPClient http;

  String serverPath = url;

  // Your Domain name with URL path or IP address with path
  http.begin(serverPath.c_str());

  // Send HTTP GET request
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String payload = http.getString();
    return payload;

  }
  else {
    return "erro";
  }
  http.end();
}

void telegramLog(String mensagem) {
  // ColeSeuTokenAqui ColeIDdoGrupoAqui TestandoEnvio
  // https://api.telegram.org/bot5199663658:AAF4D8-KtthX87TGX6pYHBiLGTTZYPyU3Z8/getUpdates
  String url = "https://api.telegram.org/bot" + chaveTelegram + "/sendMessage?chat_id=" + chat + "&text=" + mensagem;
  wget(url);
}
void telegramAlarm(String mensagem) {
  if ((millis() - lastTimeAlert) > timerAlerta) {
  // ColeSeuTokenAqui ColeIDdoGrupoAqui TestandoEnvio
  String url = "https://api.telegram.org/bot" + chaveTelegram + "/sendMessage?chat_id=" + alert + "&text=" + mensagem;
  wget(url);
  lastTimeAlert = millis(); // Esta linha sempre no final BLOCO QUE CONTROLA O TIMER!
  }
}

void eti(int num) {
  // ColeSeuTokenAqui ColeIDdoGrupoAqui TestandoEnvio
  //Vamos obter a URL perguntando também ao github
  String url = urlSite+"/sonda/?sensor=" + String(idSensor) + "&valor=" + String(num);
  Serial.println(wget(url));
}

/* Conexão com o broker e também servirá para reestabelecer a conexão caso caia */
void conectar_broker() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.println("Conectando-se ao broker mqtt...");

  uint8_t num_tentativas = 3;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Falha ao se conectar. Tentando se reconectar em 5 segundos.");
    mqtt.disconnect();
    delay(5000);
    num_tentativas--;
    if (num_tentativas == 0) {
      Serial.println("Provavelmente não tem internet.");
      break;
    }
  }

  Serial.println("Conectado ao broker com sucesso.");
}

void callback() {
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &wifiTime) {
      Serial.print(F("Got: "));
      Serial.println((char *)wifiTime.lastread);
    }
  }
}

void sonar() {
  distance = sonar1.read();
  distance = distance - distanciaSonda;
  Serial.print("Sonar -> ");
  Serial.println(distance);

  if (minimo == 0 || minimo > distance) {
    minimo = distance;
  }


  if (maximo == 0 || maximo < distance) {
    maximo = distance;
  }

  distance > alturaAgua ? progresso = alturaAgua : progresso = distance;
  progresso = 100 * progresso / alturaAgua;
  progresso = 100 - progresso;

}

void tela() {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, StatusInternet);
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 14, String(distance));
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 14, String(minimo));
  display.drawString(64, 28, String(maximo));
  //display.drawString(0, 40, String(minimo));
  //display.drawString(60, 40, String(maximo));
  display.drawString(90, 19, String(progresso));
  display.drawProgressBar(0, 40, 127, 22, progresso);
  display.display();
  delay(500);
}

void IoT() {
  internet();
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("HORA DE TAREFAS DA WEB");

      // Lê o que tiver de novo na Adafruit
      callback();
      
      String msg = nomeDaSonda + " -> Distancia do Sensor: " + String(distance) + "cm";
      telegramLog(msg);
      eti(distance);

      conectar_broker();
      getParametrosRemotos();

      if (!sonar01.publish(distance) || !sonar02.publish(progresso)){
        Serial.println("Erro ao publicar na Adafruit");
      } else {
        Serial.println("Publicado na Adafruit");
      }

      if (progresso < nivelAlerta) {
        String msg = nomeDaSonda + " -> NIVEL DE ÁGUA ABAIXO DO ESPERADO! => Reservatório está em " + String(progresso) + "%";
        telegramAlarm(msg);
      }
      //Serial.println(String(sonar01.publish(distance)));
      lastTime = millis(); // Esta linha sempre no final BLOCO QUE CONTROLA O TIMER!

    }
  }
}

void internet(){
  if (WiFi.status() == WL_CONNECTED) {
    StatusInternet = "NaRede!"+WiFi.localIP().toString();
    return;
  } else {
    StatusInternet = "Sem Wifi...";
    WiFi.begin(ssid, password);
    Serial.println("Conectando na rede WiFi");
    int i = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      if (i++ > 10) {
        break;
      }

    }
    Serial.println("Conectado com IP: ");
    Serial.println(WiFi.localIP());
  }
}

void getParametrosRemotos(){
      timerDelay = wget(IntervaloDePush).toInt();
      nivelAlerta = wget(nivelAlertaTelegram).toInt();
      urlSite = wget(nivelAlertaTelegram);
      Serial.println(timerDelay);
      Serial.println(nivelAlerta);
  }
