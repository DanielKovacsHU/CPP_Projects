# Blinking LED with a Raspberry Pi Pico 2 W

Written in the Arduino IDE, the Pico drives the LED from GP15 through a
current-limiting resistor. It is the smallest useful program you can put
on the board.

## Components and design

### Parts

- Raspberry Pi Pico 2 W
- Red LED (about 2 V forward voltage at 15 mA, 20 mA maximum)
- 100 ohm resistor
- Breadboard
- Jumper wire
- USB cable

### Electrical parameters

| Symbol | Parameter | Value |
| --- | --- | --- |
| I_led (max) | Maximum continuous forward current | 20 mA |
| I_led | Design target current | 15 mA |
| U_f | LED forward voltage at 15 mA | 2 V |
| U_pin | Pico output pin voltage | 3.3 V |
| U_R | Voltage drop by the resistor | ? |
| R | Current-limiting resistor value | ? |
| I | Actual current on the led by the chosen resistor | ? |

### Resistor value

The LED is not driven at its 20 mA limit. A target of 15 mA leaves some
headroom. The resistor has to drop the difference between the pin voltage and
the LED forward voltage.

Voltage across the resistor:

    U_R = U_pin - U_f = 3.3 V - 2 V = 1.3 V

Current through the resistor:

    I_led = 15 mA = 0.015 A

Resistance from Ohm's law:

    R = (U_pin - U_f) / I_led
      = 1.3 V / 0.015 A
      = 86.66 ohm

The nearest common value is 100 ohm. With that resistor the current is:

    I = (U_pin - U_f) / R = 1.3 V / 100 ohm = 0.013 A = 13 mA

13 mA is below the 20 mA maximum, so the LED keeps a reasonable safety margin
and is still clearly visible.

## Circuit and code

The LED anode (long leg) connects to GP15 through the 100 ohm resistor. The
cathode (short leg) connects to GND with a jumper wire.

| Pico 2 W pin | Connects to |
| --- | --- |
| 20 (GP15) | 100 ohm resistor, then LED anode |
| 23 (GND) | jumper wire, then LED cathode |

### Image of the circuit
<img width="750" height="500" alt="led_blink" src="https://github.com/user-attachments/assets/5b8465ab-72c6-4363-bc79-f432b16436c0" />

### Code:

The code is short enough to read in full, and the comments explain each line.

```cpp
const int ledpin {15};

void setup() 
{
  pinMode(ledpin, OUTPUT); // set GP15 as output
}

void loop() 
{
  digitalWrite(ledpin, HIGH); // set HIGH or 1 to GP15, which is 3.3V
  delay(1000);               // wait for 1sec
  digitalWrite(ledpin, LOW);  // set LOW or 0 to GP15, which is 0V
  delay(1000);               // wait for 1sec
}
```

## Demo

Here is a showcase how it works:

https://github.com/user-attachments/assets/a3889a39-887a-48f4-b66c-d7c0cce13566


