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
    if (digitalRead(readpin) == HIGH) // if at any point a high value is read before 500ms, loop exits, no change is made, cant use internal loop timer, must exit the loop by this condition as fix baseline time is needed before a chage in state
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


  /*
  using 1000ms because assuming immidiate change from open >> closed there was already a 50ms delay before the last state is changed so this loop starts at 500ms, if we would use 500ms, there whould be no time condition
  the above line was my initial idea, but its an extrememly unlikley case, would be only triggered if the state is changed to HIGH while in the 50ms correct loop, whould need to change while 3 line is executed inside it in around 1 micro seconds (mainly from println(), its a rough calculation based on pico2w clock speed of 7 nano second cycle time multiplied with 100 cycle for the total println(), thats 0.7 micro seconds rounded to 1 whole), 
  if dont changed to HIGH in 1 micro seconds the 2nd while loop wont trigger delayed loop ends, new loop and counter started from 0 and waiting for a HIGH value
  now using 500ms for both, this value came from testing edge cases where false positives whould trigger if i move the resistor up and down, with a down speed equal to the falling speed of the balls (similar to freefall/plane diving zero G effect, that if the body surrounding area is falling with the same speed as the body itself, relative to the area the body dont change, assuming no other forces aside from gravitation)
  */ 

  while (digitalRead(readpin) == HIGH && last_state == "open")  // if we read a HIGH value and the last state is open, a 500ms waiting loop starts
  {
    if (digitalRead(readpin) == LOW) // if at any point a LOW value is read before 500ms, loop exits, no change is made, cant use internal loop timer, must exit the loop by this condition as fix baseline time is needed before a chage in state
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
