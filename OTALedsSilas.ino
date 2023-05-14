#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#ifndef STASSID
#define STASSID "INTELBRAS"
#define STAPSK  "Pbl-Sistemas-Digitais"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
const char* host = "ESP-10.0.0.107";

int led_pin = LED_BUILTIN;
#define N_DIMMERS 3
int dimmer_pin[] = {14, 5, 15};
const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0


void setup() {
  Serial.begin(115200);

  /* switch on led */
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);

  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("Retrying connection...");
  }
  /* switch off led */
  digitalWrite(led_pin, HIGH);

  /* configure dimmers, and OTA server events */
  analogWriteRange(1000);
  analogWrite(led_pin, 990);

  for (int i = 0; i < N_DIMMERS; i++) {
    pinMode(dimmer_pin[i], OUTPUT);
    analogWrite(dimmer_pin[i], 50);
  }

  ArduinoOTA.setHostname(host);
  ArduinoOTA.onStart([]() { // switch off all the PWMs during upgrade
    for (int i = 0; i < N_DIMMERS; i++) {
      analogWrite(dimmer_pin[i], 0);
    }
    analogWrite(led_pin, 0);
  });

  ArduinoOTA.onEnd([]() { // do a fancy thing with our board led at end
    for (int i = 0; i < 30; i++) {
      analogWrite(led_pin, (i * 100) % 1001);
      delay(50);
    }
  });

  ArduinoOTA.onError([](ota_error_t error) {
    (void)error;
    ESP.restart();  
  });

  /* setup the OTA server */
  ArduinoOTA.begin();
  Serial.println("Ready");


  bool selectPlaca = true;
}

void loop() {
  ArduinoOTA.handle();

  //le valor analogico (2^10)
  analogSensorValue = analogRead(analogInPin); 
  

  if (Serial.available() > 0 ){
      char r = Serial.read();

      if (r == 10000001){
        selectPlaca = true
        char sensorSelect = Serial.read();
        if (sensorSelect == 10000001){ // Escolheu o sensor analógico
          int b1 = (analogSensorValue >> 5); //Pega os 5 bits mais significativos
          int b2 = (analogSensorValue & 0b00000011111); //Pega os 5 bits menos significativos
          Serial.Write(b1);
          Serial.Write(b2);
        }
      }
      if (r == 00000001){
        selectPlaca = false
      }

      if (selectPlaca){
        //
      }

      if (r = 10000002)

  }


  // if (Serial.available() > 0 ){
  //     char r = Serial.read();

  //     if (r == 10000001){
  //       while(1){
  //         r = Serial.read();

  //         if (r == 00000001){
  //           break;
  //         }
  //       }

  //     }
  // } 
} 
