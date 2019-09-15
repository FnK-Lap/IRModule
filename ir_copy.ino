// IR
#include <IRremoteESP8266.h> // https://github.com/crankyoldgit/IRremoteESP8266
#include <IRrecv.h>
#include <IRsend.h>

#include <IRac.h>
#include <IRUtils.h>

// WebServer
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

// Wifi
#include <ESP8266WiFi.h> // https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/station-class.html#status

ESP8266WebServer server(80);

// ================================================== //
// ====================== Setup ===================== //
// ================================================== //
void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Setup start");

  initWiFi();
  initWebServer(WiFi.localIP());
  initIR();
}

// ================================================== //
// ====================== Loop ====================== //
// ================================================== //
void loop() {
  server.handleClient();
}
