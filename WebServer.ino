MDNSResponder mdns;

#define HOSTNAME "esp32"
// -------------------------------------------------- //
// ----------------- Init Web Server ---------------- //
// -------------------------------------------------- //
void initWebServer(IPAddress ip) {
  Serial.print("Starting MDNS : ");

  if (mdns.begin(HOSTNAME, ip)) {
    Serial.println("MDNS responder started");
  }

  // https://links2004.github.io/Arduino/d3/d58/class_e_s_p8266_web_server.html#a30d947fd2c8d6fa5d9ca421c558a3238
  server.on("/record", handleRecord);
  server.on("/send", handleSend);
  server.onNotFound(handleNotFound);

  Serial.print("Starting WebServer : ");
  server.begin();
  Serial.println("Started");
}

// Handle Not Found HTTP Request
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  server.send(404, "text/plain", message);
}

// Handle Record page
void handleRecord() {
  Serial.println("Record");
  int maxRetry = 10;
  
  if(server.hasArg("retry")) {
    maxRetry = server.arg("retry").toInt();
  }

  if (maxRetry < 1 || maxRetry > 10) {
    server.send(400, "text/plain", "Erreur ! FU .|.. ..|.");
    Serial.println("Aborted. Parameters : ");
    Serial.println(server.arg("retry"));

    return;
  }
  
  server.send(200, "text/plain", "Record started");

  String IRValues[maxRetry];
  int IRValuesCount[maxRetry];
  int nbRetry = 0;
  
  int nbGoodValues = 0;
  int nbGoodValuesRequired = 3;

  while (nbRetry < maxRetry) {
    Serial.print(".");
    if (irrecv.decode(&IRResults)) {
      long IRCode = IRResults.value;
      String IRCodeHex = String(IRCode, HEX);
      Serial.println("");
      Serial.printf("Received value %d/%d : %s\n", nbRetry + 1, maxRetry, IRCodeHex.c_str());
      Serial.printf("Overflow : %s\n", IRResults.overflow ? "true" : "false");
      Serial.printf("Repeat : %s\n", IRResults.repeat ? "true" : "false");
      Serial.printf("Encoding (code) : %d\n", IRResults.decode_type);
      Serial.printf("Code : %d\n", IRResults.value);
      // Display the basic output of what we found.
      Serial.print(resultToHumanReadableBasic(&IRResults));
      // Display any extra A/C info if we have it.
      String description = IRAcUtils::resultAcToString(&IRResults);
      if (description.length()) Serial.println("Mesg Desc.: " + description);
      yield();  // Feed the WDT as the text output can take a while to print.
      // Output RAW timing info of the result.
      Serial.println(resultToTimingInfo(&IRResults));
      yield();  // Feed the WDT (again)
      // Output the results as source code
      Serial.println(resultToSourceCode(&IRResults));
      Serial.println();    // Blank line between entries
      yield();             // Feed the WDT (again)

      IRValues[nbRetry] = IRCodeHex;
      IRValuesCount[nbRetry] = 1;

      for (int index = 0; index < maxRetry; index++) {
        if (nbRetry != index && IRValues[index] == IRCodeHex) {
          IRValuesCount[index]++;
        }
      }
      
      irrecv.resume();
      ++nbRetry;
    }
    delay(200);
  }

  int maxValue = 0;
  int maxIndex;

  for (int index = 0; index < maxRetry; index++) {
    if (IRValuesCount[index] > maxValue) {
      maxValue = IRValuesCount[index];
      maxIndex = index;
    }
  }

  Serial.println("Record Done !");
  Serial.print("Hex code : ");
  Serial.print(IRValues[maxIndex]);
  Serial.printf(" | Matches %d/%d\n", maxValue, maxRetry);
}

// Handle Send page
void handleSend() {
  Serial.println("Send");

  if (
       false == server.hasArg("code") 
    || false == server.hasArg("encoding")
  ) {
    server.send(400, "text/plain", "Missing parameters.");
  }

  String code = server.arg("code");
  uint64_t cc;
  sscanf(code.c_str(), "%I64x", &cc);
  
  decode_type_t encoding = (decode_type_t)server.arg("encoding").toInt();
  uint16_t size = server.arg("size").toInt();

  irsend.send(encoding, cc, size);
//
//  switch (encoding) {
//    default:
//      Serial.print("Bad encoding received");
//      server.send(400, "text/plain", "Bad encoding received");
//      break;
//  }
//
//  Serial.println("Code to send :");
//  Serial.printf("Code : %s", code.c_str());
//  irsend.send(encoding, code.c_str());
//  ff02fd

  server.send(200, "text/plain", "Ok");
}
