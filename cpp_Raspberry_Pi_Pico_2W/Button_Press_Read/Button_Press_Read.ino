#include <string>

// variable pin will be gpio 15
const int pin {15};

void setup() 
{
  pinMode(pin, INPUT);  //setting the pin as input
  Serial.begin(1); // can use any value except 1200 which enters bootsel mode
}

std::string last_state {"LOW"}; // std::string is the modern cpp class for variable lenght strings

void loop() 
{
  // only shows high read value if the previous value (last_state) was "low" and the button is pressed (reads HIGH), this will avoid longpress resulted multiple inputs
  if (digitalRead(pin) == HIGH && last_state == "LOW")
  {
    Serial.println("Input reads HIGH value");
    last_state = "HIGH";
  }


  // resets the last_sate to "low" if the previous value was "high" and the button was released (reads LOW), (no need to assign "low", if its already "low")
  if (digitalRead(pin) == LOW && last_state == "HIGH")
  {
    last_state = "LOW";
  }

  delay(50); // small delay to avoid false reads while the button is floating
}
