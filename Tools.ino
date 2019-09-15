/**
 * WiFi Ip -> String
 */
String ip2Str(IPAddress ip){
  String s = "";
  for (int i = 0; i < 4; i++) {
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  }
  return s;
}

/**
 * WiFi Status -> Label
 */
String WiFiStatus2Label(int WiFiStatus) {
  String label = "";
  
  switch(WiFiStatus) {
      case WL_NO_SSID_AVAIL:
       label = "SSID cannot be reached";
        break;
      case WL_CONNECT_FAILED:
        label = "Wrong password";
        break;
      case WL_IDLE_STATUS:
        label = "WiFi changing status";
        break;
      case WL_DISCONNECTED:
        label = "Disconnected";
        break;
      case WL_CONNECTED:
        label = "Connected";
        break;
      default:
        label = "Unknown status";
        break;
    }

    return label;
}

/**
 * Print WiFi Info
 */
void printWiFiInfo() {
  Serial.printf("Status : %d - %s\n", WiFi.status(), WiFiStatus2Label(WiFi.status()).c_str());
  Serial.printf("SSID: %s\n", WiFi.SSID().c_str());
  Serial.printf("Local IP: %s\n",  ip2Str(WiFi.localIP()).c_str());
  Serial.printf("Mac Address: %s\n", WiFi.macAddress().c_str());
  Serial.printf("Hostname: %s\n", WiFi.hostname().c_str());
  Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
}
