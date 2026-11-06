#include <string>   // to use std::string type

const int readpin {15};   // readpin variable with value 15

void setup() 
{
  pinMode(readpin, INPUT);  // set pin 15 to input mode
  Serial.begin(1);  // begin serial communication
}

std::string last_state {"low"};   // start with a low state (not triggered)
unsigned long detection_start {};   // value initization of the variable

void loop() 
{
  if (digitalRead(readpin) == HIGH && last_state == "low")    // if currently there is a trigger (high value) and the last state is "low"
  {
    detection_start = millis();   // set the detection_start variable to the current time
    last_state = "high";    //  set the last_state variable to "high"
    Serial.println("Motion is detection started");    //  print out the message
  }

  if (digitalRead(readpin) == LOW && last_state == "high")    // if currently there is no trigger (low value read) and the last state is "high"
  {
    last_state = "low";   // set last_state to "low"
    Serial.println("Motion is detection ended after " + String((millis() - detection_start) / 1000) + "sec");   // print out the high signal time (how long was the movement), including the value hold delay
  }
}
