#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include <ArduinoJson.h>

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);

int loopCnt = 0;


/**
 * Establish WiFi-Connection
 */
void startWIFI() {
  Serial.println("---");
  WiFi.mode(WIFI_STA);
  Serial.println("(Re)Connecting to Wifi-Network with following credentials:");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Key: ");
  Serial.println(password);
  Serial.print("Device-Name: ");
  Serial.println(espName);
  
  WiFi.hostname(espName);
  WiFiMulti.addAP(ssid, password);

  int tryCnt = 0;
  
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tryCnt++;
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(300);
}


/**
 * Unknown Route
 * Send teapot.
 */
void handleRequestNotFound(){
  server.send(418, "text/plain", "I'm a door.");
}
