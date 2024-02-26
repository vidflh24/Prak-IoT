int ledPin1 = 14;

void setup() {
  pinMode(ledPin1, OUTPUT);

}

void loop() {
  digitalWrite(ledPin1, HIGH);
  delay(5000);
  digitalWrite(ledPin1, LOW);
  delay(1000);

}
