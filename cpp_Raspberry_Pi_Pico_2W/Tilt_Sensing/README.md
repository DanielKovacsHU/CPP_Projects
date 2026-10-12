# Tilt detection with a Raspberry Pi Pico 2 W

A ball tilt switch on GP15 is read by a Raspberry Pi Pico 2 W. The switch is closed while the board sits upright and opens when it is tilted. Each accepted change of position prints one message to the serial monitor. The sketch runs in the Arduino IDE. On the Pico 2 W the serial port is not a hardware UART; it is the USB CDC port, which creates a virtual COM port over the USB connection, so no GPIO pins are used for it.

## Components and design

### Parts

- Raspberry Pi Pico 2 W
- Tilt switch (ball type)
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

The switch connects the pin to 3.3 V when it is closed, and the 10k ohm resistor pulls it back to GND when it is open. A 10k ohm value is a common choice for this job. It is a balance between a low resistance, which wastes more current while the switch is closed, and a high resistance, which is more prone to noise and leakage current. The 3.3 V comes from the Pico 2 W 3V3(OUT) pin.

The pin is set to plain INPUT rather than INPUT_PULLUP, because the pulldown is provided externally by that resistor.

### Tilt switch

A ball tilt switch is a small metal ball inside a tube with two contacts at one end. Upright, the ball rests on the contacts and bridges them, so the switch is closed and GP15 reads HIGH. Tilted far enough, the ball rolls away from the contacts, the switch opens, and the pulldown brings GP15 to LOW.

The names in the code follow the switch, not the pin. "closed" is the bridged, upright position and "open" is the tilted position. The initial value of `last_state` assumes the board starts upright.

### Serial port

On the Pico 2 W, `Serial` is the USB CDC (Communication Device Class) port and not a hardware UART. It creates a virtual COM port directly over the USB connection to the computer, so no GPIO pins are involved at all. For USB CDC serial ports (e.g. Serial on the Leonardo), Serial.begin() is irrelevant. You can use any baud rate and configuration for serial communication with these ports.

[source: Arduino](https://docs.arduino.cc/language-reference/en/functions/communication/serial/begin/#:~:text=For%20USB%20CDC,with%20these%20ports)

The one exception is 1200 baud, which puts the board into BOOTSEL mode.

## Time measurement and the 500 ms window

A mechanical tilt switch does not switch cleanly. As the ball rolls it can touch the contacts for a moment, break them again, and only then settle. A single fast read is therefore not a reliable sign of the final position, so the sketch accepts a reading only after it has held steady for a window.

The timing is measured with `millis()`, which returns the number of milliseconds since the board started and keeps counting on its own without stopping the sketch. At the top of `loop()` the code stores that value in `loop_start_time`, the start of the window. Inside the waiting loop, each pass compares the current time with the stored start:

    millis() - loop_start_time >= 500

The subtraction gives how long has passed since the window opened. Doing it as a difference, instead of comparing millis() with a fixed value, also stays correct when millis() wraps around after about 49.7 days.

Inside the loop, if the pin reads back at any point before the 500 ms are up, the loop exits and no change is made. Only a level that stays put for the full 500 ms is taken as a true tilted position or a true untilted/closed position, printed, and stored.

That is why the internal loop timer cannot be used for the wait. delay(500) would stop the sketch for the whole window, so the pin could not be read while it runs and a reading that changes back would be missed. The waiting loop reads the pin on every pass instead.

Why 500 ms
The value came from testing edge cases where a false positive could trigger. Moving the sensor up and down can produce one, and the worst case is a fast downward move. If the sensor is moved down at the same speed the ball would fall, the ball does not move relative to the tube. This is the same as a body falling together with its surroundings, like the dive a plane makes to simulate zero G: if the surroundings fall at the same speed as the body, then relative to the outside the body does not change position, assuming gravity is the only force acting on it.

Moved that way, the ball floats instead of settling. In testing the float lasted about 400 to 500 ms before the downward move stopped and the ball dropped back into place. A window shorter than that would take the float for a real change. 500 ms is used for both waiting loops, the tilted one and the upright one, so a float in either direction is ignored while a position that is actually held is still reported.

A normal shake, or the ball settling on its own, is over in tens of milliseconds; the float is the slow case that sets the number.

## Circuit and code

The tilt switch sits between the 3V3(OUT) pin and GP15. The 10k ohm resistor
sits between GP15 and GND.

| Pico 2 W pin | Connects to |
| --- | --- |
| 36 (3V3(OUT)) | One side of the tilt switch |
| 20 (GP15) | Other side of the tilt switch, and one end of the 10k ohm resistor |
| 18 (GND) | Other end of the 10k ohm resistor |

### Schematic

<img width="429" height="273" alt="tilt switch" src="https://github.com/user-attachments/assets/83c3ef46-7685-4b22-b70c-a0baa1300bdb" />


### Assembled circuit

<img width="600" height="452" alt="circuit" src="https://github.com/user-attachments/assets/e1da9fff-8b80-4017-961e-1cfd096938fe" />


### The code:

`last_state` holds the last accepted position as a `std::string`, either "closed" or "open". Two waiting loops watch for a change. The first runs while
the pin reads LOW and the last state was "closed"; the second runs while the pin reads HIGH and the last state was "open". Each one starts its own 500 ms window from `loop_start_time` and prints and updates the state only when the new reading holds for the full window. A state change ends the loop, and the next `loop()` records a fresh `loop_start_time`, so every transition is timed on its own.

```cpp
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
```

## Demo

Shows a brief shake being ignored, then a held tilt and a return to upright each printing one message.

https://github.com/user-attachments/assets/d29609d0-df90-45b6-ad0d-224a57611702

Small note: it seems like the tilt back closed position sensing is taking longer than the opening tilt motion. This might be true, because the usb cdc printing out can add latency. I tested the internal correctness and timing by printing out times, it's working exactly as planed, each change is taking at least 500ms time. The image shows the start time of the new input and the end time its considered a true signal.

<img width="295" height="230" alt="Arduino_IDE_RrNm9nkbZE" src="https://github.com/user-attachments/assets/c92d9456-2ed0-4ed0-b721-0b86dac97188" />



