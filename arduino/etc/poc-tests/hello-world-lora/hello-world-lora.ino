int num;

void setup() {
  // put your setup code here, to run once:
  Serial.println("Setup mode.");
  num = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  num++;
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);         
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
  delay(100);       
  Serial.print("Blinky. ");
  Serial.println(num);
}
