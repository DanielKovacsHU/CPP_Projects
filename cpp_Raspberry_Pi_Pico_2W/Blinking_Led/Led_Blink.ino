const int ledpin = 15;

void setup() 
{
  pinMode(ledpin, OUTPUT); // set GP15 as output
}

void loop() 
{
  digitalWrite(ledpin, HIGH); // set HIGH or 1 to GP15, which is 3.3V
  delay(1000);               // wait for 1sec
  digitalWrite(ledpin, LOW);  // set LOW or 0 to GP15, which is 0V
  delay(1000);               // wait for 1sec
}
