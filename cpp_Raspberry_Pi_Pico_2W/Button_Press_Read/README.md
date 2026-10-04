# Push button input with a Raspberry Pi Pico 2 W

A push button on GP15 is read by a Raspberry Pi Pico 2 W. Each time the button
is pressed, a message is printed to the serial monitor. The sketch runs in the
Arduino IDE. On the Pico 2 W the serial port is not a hardware UART; it is the
USB CDC port, which creates a virtual COM port over the USB connection, so no
GPIO pins are used for it.

## Components and design

### Parts

- Raspberry Pi Pico 2 W
- Push button (4 leg version)
- 10k ohm resistor (pulldown)
- Breadboard
- Jumper wires
- USB cable

### Electrical parameters

| Symbol | Parameter | Value |
| --- | --- | --- |
| U_pin | Logic high level | 3.3 V from the 3V3(OUT) pin |
| R_pulldown | Pulldown resistor | 10k ohm |
| Pin mode | Input mode | INPUT (external pulldown) |

### Pulldown resistor

The button pulls the pin to 3.3 V when pressed, and the 10k ohm resistor pulls
it back to GND when released. A 10k ohm value is a common choice for this job.
It is a balance between a low resistance, which wastes more current while the
button is held, and a high resistance, which is more prone to noise and leakage
current. The 3.3 V comes from the Pico 2 W 3V3(OUT) pin.

The pin is set to plain INPUT rather than INPUT_PULLDOWN, because the pulldown
is provided externally by that resistor.

### Serial port

On the Pico 2 W, `Serial` is the USB CDC (Communication Device Class) port and
not a hardware UART. It creates a virtual COM port directly over the USB
connection to the computer, so no GPIO pins are involved at all. For USB CDC
serial ports (e.g. Serial on the Leonardo), Serial.begin() is irrelevant. You
can use any baud rate and configuration for serial communication with these
ports.

[source: Arduino](https://docs.arduino.cc/language-reference/en/functions/communication/serial/begin/#:~:text=For%20USB%20CDC,with%20these%20ports)

The one exception is 1200 baud, which puts the board into BOOTSEL mode.

`println` prints data to the serial port as human-readable ASCII text followed
by a carriage return character (ASCII 13, or '\r') and a newline character
(ASCII 10, or '\n').

## Circuit and code

The button sits between the 3V3(OUT) pin and GP15. The 10k ohm resistor sits
between GP15 and GND.

| Pico 2 W pin | Connects to |
| --- | --- |
| 36 (3V3(OUT)) | One side of the button |
| 20 (GP15) | Other side of the button, and one end of the 10k ohm resistor |
| 18 (GND) | Other end of the 10k ohm resistor |

### Image of the circuit:

<img width="750" height="500" alt="button_press" src="https://github.com/user-attachments/assets/063eb59d-adea-49dc-9d39-674e58535452" />

### The code:
The code keeps the previous pin level in `last_state`. A message is only
printed when the pin goes from LOW to HIGH, so holding the button down does not
produce repeated messages. The state is set back to LOW on release. The 50 ms
delay at the end of the loop reduces false reads.

```cpp
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
```

### Input stability

Only relevant if the input is not a stable 3.3V high value.

[e9 issue](https://pip-assets.raspberrypi.com/categories/1214-rp2350/documents/RP-008373-DS-2-rp2350-datasheet.pdf#page=1367&zoom=100,153,374)

## Demo
Showcases that it can read single short and long inputs and double presses without issue.

https://github.com/user-attachments/assets/0c2d8e89-6d95-4349-836b-1712628e3c46



