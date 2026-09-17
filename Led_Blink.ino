void setup() 
{
  pinMode(15, OUTPUT); // set GP15 as output
}

void loop() 
{
  digitalWrite(15, HIGH); // set HIGH or 1 to GP15, which is 3.3V
  delay(1000);               // wait for 1sec
  digitalWrite(15, LOW);  // set LOW or 0 to GP15, which is 0V
  delay(1000);               // wait for 1sec
}
