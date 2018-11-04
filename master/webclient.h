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
 * Fire request via Post
 * and include given payload
 */
void sendPostRequest(HTTPClient &client, int &httpCode, String url, String payload) {
  client.begin(url);
  Serial.print("[HTTP] URL: ");
  Serial.println(url);
  Serial.print("[HTTP] Payload: ");
  Serial.println("payload=" + payload);
  Serial.println("[HTTP] Requesting...");
  client.setUserAgent(userAgent + " " + clientVer);
  client.addHeader("Content-Type", contenttype);
  httpCode = client.POST(payload);
  //client.writeToStream(&Serial); //DEBUG ONLY
}


void sendUpdate(String updateString) {
  String url = "http://" + slaveIp + "/" + updateString;
  String payload = "";
  Serial.println("---");
  Serial.println("[HTTP] Start connection info request...");

  Serial.println("[HTTP] Sending data.");
  HTTPClient http;
  int httpCode = -1;

  Serial.print("[HTTP] URL: ");
  Serial.println(url);
  Serial.print("[HTTP] Payload: ");
  Serial.println(payload);
  Serial.println("[HTTP] Requesting...");
  sendPostRequest(http, httpCode, url, payload);
}


/**
 * Unknown Route
 * Send teapot.
 */
void handleRequestNotFound(){
  server.send(418, "text/plain", "I'm a door.");
}


/**
 * Start webserver for handling
 * incoming requests
 */
void startWebserver() {
  Serial.println("Starting HTTP-Server...");
  Serial.println("-- Registering routes.");
  server.onNotFound(handleRequestNotFound);
  Serial.println("-- Launching server ...");
  server.begin();
  Serial.println("-- DONE.");
}
