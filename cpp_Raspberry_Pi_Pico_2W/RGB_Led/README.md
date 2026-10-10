# RGB LED color fade with a Raspberry Pi Pico 2 W

An RGB LED cycles through blended colors on a Raspberry Pi Pico 2 W. The sketch
runs in the Arduino IDE and drives the three LED legs from GP13 (red), GP14
(green), and GP15 (blue), each through its own current-limiting resistor. PWM
sets the brightness of each leg, and the firmware shifts them against each other
to move between colors.

## Components and design

### Parts

- Raspberry Pi Pico 2 W
- RGB LED (common cathode), 20 mA maximum per leg
- 3 x 100 ohm resistor
- Breadboard
- Jumper wire
- USB cable

### Electrical parameters

| Symbol | Parameter | Value |
| --- | --- | --- |
| I_f | Maximum continuous forward current per leg | 20 mA |
| U_pin | Pico output pin voltage | 3.3 V |
| U_f red | Red forward voltage | 2 V |
| U_f green | Green forward voltage | 3 V |
| U_f blue | Blue forward voltage | 3 V |
| R_series_red | Current-limiting resistor for the red leg | ? |
| R_series_blue | Current-limiting resistor for the blue leg | ? |
| R_series_green | Current-limiting resistor for the green leg | ? |
| I_red | Actual current on the red leg | ? |
| I_blue | Actual current on the blue leg | ? |
| I_green | Actual current on the green leg | ? |


### Resistor values

Each leg gets one series resistor, sized so the voltage left across the LED
stays at or below its forward voltage at the maximum continuous current.

Red:

    R_series_red = (U_pin - U_f_red) / I_f
                 = (3.3 V - 2 V) / 0.02 A
                 = 65 ohm

The nearest value available is 100 ohm. A larger resistance lowers the current,
but the forward voltage at that lower current is not known. Assuming a 10%
drop, the red forward voltage is about 1.8 V, so the current becomes:

    I_red = (U_pin - U_f_red) / R_series_red
          = (3.3 V - 1.8 V) / 100 ohm
          = 1.5 V / 100 ohm
          = 15 mA

15 mA is below the 20 mA limit, so red is safe.

Blue:

    R_series_blue = (U_pin - U_f_blue) / I_f
                  = (3.3 V - 3 V) / 0.02 A
                  = 15 ohm

The nearest value available is again 100 ohm (the smaller 10 ohm resistor would push the current to 30mA above the limit), which is much larger than required. Assuming a 15% drop, the blue forward voltage is about 2.55 V, so the current becomes:

    I_blue = (U_pin - U_f_blue) / R_series_blue
           = (3.3 V - 2.55 V) / 100 ohm
           = 0.75 V / 100 ohm
           = 7.5 mA

Green uses the same numbers as blue:
    
    I_green = 7.5 mA
    R_series_green = 100 ohm

Both blue and green run at about 7.5 mA, which is far below the 20 mA maximum, so
those two legs are much dimmer than red. Balancing the brightness would need
smaller resistors on the blue and green legs, smaller than 100 ohm but bigger than 15 ohm.

## Circuit and code

Each leg connects to its own pin through a 100 ohm resistor. The common cathode (higher duty cycle makes a leg brighter)
goes to GND with a jumper wire.

| Pico 2 W pin | Connects to |
| --- | --- |
| 17 (GP13) | 100 ohm resistor, then red leg |
| 19 (GP14) | 100 ohm resistor, then green leg |
| 20 (GP15) | 100 ohm resistor, then blue leg |
| 23 (GND) | jumper wire, then common cathode leg |

### Image of the circuit:

<img width="750" height="500" alt="led_rgb_1" src="https://github.com/user-attachments/assets/5c3386cd-bade-4636-b41a-3f3a911a6d30" />

### Code:
Each leg is driven with PWM, so the duty cycle sets that leg's brightness. The
helper function writes all three legs at once, and the three loops in `loop()`
raise one leg while lowering another to slide between colors.

```cpp
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

  // red stay, green dimmer, blue brighter (Red - Blue)
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
```

## Demo

The LED moves from red - green (yellow), then red - blue (magenta), then green - blue (cyan), pausing
briefly on each step before repeating.

https://github.com/user-attachments/assets/21a2a2ff-e691-44dd-8f63-a81ec17dcf73

For better visualization I used a folded paper as a light diffuser, showcasing the 3 blended color of yellow, cyan and magenta

https://github.com/user-attachments/assets/a51dcc8d-16c0-4efd-8957-60abadaa0acd


