const int ledpin {15};

void setup() 
{
  pinMode(ledpin, OUTPUT); // gpio 15 as output
}

void loop() 
{
  for (int pwm {0}; pwm < 256; ++pwm) // loop for increasing the pwm value from 0 to 255 in 1 increments
  {
    analogWrite(ledpin, pwm); // output the duty cycle 0 is 0%, 255 is 100%
    delay(4); // delay of 4ms
  }

  for (int pwm {255}; pwm >= 0; --pwm)  // loop for decreasing the pwm value from 255 to 0 in 1 increments
  {
    analogWrite(ledpin, pwm);  // output the duty cycle 0 is 0%, 255 is 100%
    delay(4);  // delay of 4ms
  }
}
