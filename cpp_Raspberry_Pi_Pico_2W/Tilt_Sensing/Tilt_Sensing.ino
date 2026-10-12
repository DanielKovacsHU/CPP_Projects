# include <string>

const int readpin {15}; // pin 15 as a variable for input reading

void setup() 
{
  pinMode(readpin, INPUT);  // set pin 15 as input
  Serial.begin(1); // starting a serial communication, can be used with any value except 1200
}

std::string last_state {"closed"};  // the initial value of last_state is closed, assuming upright position (closed legs)

void loop() 
{
  unsigned long loop_start_time {millis()}; // the loop start time is X

  while (digitalRead(readpin) == LOW && last_state == "closed")  // if we read a low value and the last state is closed, a 500ms waiting loop starts
  {
    if (digitalRead(readpin) == HIGH) // if at any point a high value is read before 500ms, loop exits, no change is made, cant use internal loop timer
    {
      break;
    }

    if (digitalRead(readpin) == LOW && (millis() - loop_start_time) >= 500)  // if after 500ms (Y current time - X start time) the value is still low, its a true tilted postition
    {
      Serial.println("Tilt detected, the circuit is open"); // print the message "Tilt detected, the circuit is open"
      last_state = "open";  // the last state is now open
      break;
    }
  }



  while (digitalRead(readpin) == HIGH && last_state == "open")  // if we read a HIGH value and the last state is open, a 500ms waiting loop starts
  {
    if (digitalRead(readpin) == LOW) // if at any point a LOW value is read before 500ms, loop exits, no change is made, cant use internal loop timer
    {
      break;
    }

    if (digitalRead(readpin) == HIGH && (millis() - loop_start_time) >= 500)  // if after 500ms (Y current time - X start time) the value is still HIGH, its a true untilted/closed postition
    {
      Serial.println("The circuit is closed"); // print the message "The circuit is closed"
      last_state = "closed";  // the last state is now closed
      break;
    }
  }
}
