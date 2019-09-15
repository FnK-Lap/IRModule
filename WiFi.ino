const String wifiSSID = "Freebox-8989E4";
const String wifiPassword = "Coucou, tu veux voir ma bite ?";

// -------------------------------------------------- //
// -------------------- Init WiFi ------------------- //
// -------------------------------------------------- //
void initWiFi() {
  Serial.println("Connect WiFi\n");
  Serial.printf("SSID : %s\n", wifiSSID.c_str());
  Serial.printf("Password : %s\n", wifiPassword.c_str());
  
  WiFi.persistent(false);
  WiFi.disconnect(true);
  WiFi.begin(wifiSSID, wifiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(WiFiStatus2Label(WiFi.status()));
    delay(500);
  }

  Serial.println("Connected !");
  printWiFiInfo();
}
