const int redpin {13};
const int greenpin {14};
const int bluepin {15};

void setup() 
{
  pinMode(redpin, OUTPUT);  // gpio 13 as output (red led leg)
  pinMode(greenpin, OUTPUT);  // gpio 14 as output (green led leg)
  pinMode(bluepin, OUTPUT);  // gpio 15 as output (blue led leg)
}


// a void return function with 3 input arguments, where each is representing 1 legs pwn duty cycle
void rgb(unsigned int red, unsigned int green, unsigned int blue)
{
  analogWrite(redpin, red);  // write the input red leg duty cycle to the output
  analogWrite(greenpin, green);  // write the input green leg duty cycle to the output
  analogWrite(bluepin, blue);  // write the input blue leg duty cycle to the output
}


void loop() 
{
  // green stay, blue dimmer, red brighter (Red - Green)
  for (int bluered_index {0}; bluered_index < 256; ++bluered_index)
  {
    rgb(bluered_index, 255, 255 - bluered_index);
    delay(3);
  }

  delay(500);

  // red stay, green dimmer, blue brigher (Red - Blue)
  for (int greenblue_index {0}; greenblue_index < 256; ++greenblue_index)
  {
    rgb(255, 255 - greenblue_index, greenblue_index);
    delay(3);
  }

  delay(500);

  // blue stay, red dimmer, green brighter (Green - Blue)
  for (int redgreen_index {0}; redgreen_index < 256; ++redgreen_index)
  {
    rgb(255 - redgreen_index, redgreen_index, 255);
    delay(3);
  }

  delay(500);
}
