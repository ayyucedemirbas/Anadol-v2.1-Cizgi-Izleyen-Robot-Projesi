#include <Arduino.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
//Kütüphane Tanımlamaları
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266mDNS.h>
  
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
  //Motor sürücü pinleri tanımlandı
  const int sag_i = D2; //Sağ tekerin ileri hareketi
  const int sag_g = D3; //Sağ tekerin geri hareketi
  const int sol_i = D4; //Sol tekerin ileri hareketi
  const int sol_g = D5; //Sol tekerin geri hareketi 

void setup() {

// motor sürücü pinleri çıkış olarak atandı
    pinMode(sag_i, OUTPUT); 
    pinMode(sag_g, OUTPUT);
    pinMode(sol_i, OUTPUT);
    pinMode(sol_g, OUTPUT);
    
    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();
    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
    }

    WiFiMulti.addAP("SUPERONLINE-WiFi_7385", "KKFXN9TEPR4W");
  
}

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("http://192.168.43.165/gomulu/index.php?mode=get"); //HTTP
        http.begin("http://senolkarakurt.tk/gomulu/index.php?mode=get"); //HTTP
        delay(1000);
        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
            
            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
                if(payload == "2")//Sol
                {
                  digitalWrite(sol_g, LOW);
                  digitalWrite(sol_i, LOW);
                  digitalWrite(sag_i, HIGH);
                  digitalWrite(sag_g, LOW); 
                }
                else if(payload == "3")//Sağ
                {
                  digitalWrite(sol_g, HIGH);
                  digitalWrite(sol_i, LOW);
                  digitalWrite(sag_i, LOW);
                  digitalWrite(sag_g, LOW);
                }else if(payload == "1")//ileri
                {
                  digitalWrite(sag_g, LOW);
                  digitalWrite(sag_i, HIGH);
                  digitalWrite(sol_i, LOW);
                  digitalWrite(sol_g, HIGH);
                }else if(payload == "5")//Geri
                {
                  digitalWrite(sol_i, LOW);
                  digitalWrite(sol_g, LOW);
                  digitalWrite(sag_i, LOW);
                  digitalWrite(sag_g, LOW); 
                }else if(payload == "4")//Sabit
                {
                    digitalWrite(sag_g, HIGH);
                    digitalWrite(sag_i, LOW);
                    digitalWrite(sol_i, HIGH);
                    digitalWrite(sol_g, LOW);
                }
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

}

