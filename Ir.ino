const uint16_t IRRecvPin = 2;
IRrecv irrecv(IRRecvPin);
decode_results IRResults;

const uint16_t IRSendPin = 0;
IRsend irsend(IRSendPin, true); 
// -------------------------------------------------- //
// --------------------- Init IR -------------------- //
// -------------------------------------------------- //
void initIR() {
  // Start receiver
  Serial.print("Start IR receiver on PIN ");
  Serial.println(IRRecvPin);
  irrecv.enableIRIn();

  Serial.printf("Start IR emitter on PIN %d\n", IRSendPin);
  irsend.begin();
}
