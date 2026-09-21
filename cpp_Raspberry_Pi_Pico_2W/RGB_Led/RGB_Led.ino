const int redpin {13};
const int greenpin {14};
const int bluepin {15};

void setup() 
{
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
}

void rgb(unsigned int red, unsigned int green, unsigned int blue)
{
  analogWrite(redpin, red);
  analogWrite(greenpin, green);
  analogWrite(bluepin, blue);
}


void loop() 
{
  // green stay, blue dimmer, red brighter (RG)
  for (int bluered_index {0}; bluered_index < 256; ++bluered_index)
  {
    rgb(bluered_index, 255, 255 - bluered_index);
    delay(3);
  }

  delay(500);

  // red stay, green dimmer, blue brigher (RB)
  for (int greenblue_index {0}; greenblue_index < 256; ++greenblue_index)
  {
    rgb(255, 255 - greenblue_index, greenblue_index);
    delay(3);
  }

  delay(500);

  // blue stay, red dimmer, green brighter (GB)
  for (int redgreen_index {0}; redgreen_index < 256; ++redgreen_index)
  {
    rgb(255 - redgreen_index, redgreen_index, 255);
    delay(3);
  }

  delay(500);
}
