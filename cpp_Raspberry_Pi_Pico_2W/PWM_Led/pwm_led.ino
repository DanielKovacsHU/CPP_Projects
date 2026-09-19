const int ledpin {15};

void setup() 
{
  pinMode(ledpin, OUTPUT);
}

void loop() 
{
  for (int pwm {0}; pwm < 256; ++pwm)
  {
    analogWrite(ledpin, pwm);
    delay(4);
  }

  for (int pwm {255}; pwm >= 0; --pwm)
  {
    analogWrite(ledpin, pwm);
    delay(4);
  }
}
