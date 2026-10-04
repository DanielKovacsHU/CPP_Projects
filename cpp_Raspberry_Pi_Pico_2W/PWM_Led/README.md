# PWM LED fade with a Raspberry Pi Pico 2 W

A red LED fades up and down on a Raspberry Pi Pico 2 W. The sketch runs in the
Arduino IDE and uses PWM on GP15 to drive the LED through a current-limiting
resistor. Instead of switching the LED fully on and off, it sweeps the
brightness from zero to full and back, then repeats.

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

13 mA is below the 20 mA maximum, so the LED keeps a reasonable safety margin.
That current is the value at full brightness (100% duty cycle). At lower duty
cycles the average current is lower.

## Circuit and code

The LED anode (long leg) connects to GP15 through the 100 ohm resistor. The
cathode (short leg) connects to GND with a jumper wire.

| Pico 2 W pin | Connects to |
| --- | --- |
| 20 (GP15) | 100 ohm resistor, then LED anode |
| 23 (GND) | jumper wire, then LED cathode |

### Image of the circuit

<img width="750" height="500" alt="led_pwm" src="https://github.com/user-attachments/assets/f0868231-8eff-4180-b024-493b18b90057" />

### Duty cycle

PWM turns the pin on and off many times per second. The duty cycle is the share
of each cycle the pin spends on. analogWrite takes a value from 0 to 255: 0
keeps the pin off (0%), 255 keeps it on (100%), and 128 is on about half the
time (50%). Because the switching is faster than the eye can follow, the LED
looks like it is at a steady brightness set by the average.

The code ramps that value from 0 up to 255, then back down to 0, which is what
creates the fade.

```cpp
const int ledpin {15};

void setup() 
{
  pinMode(ledpin, OUTPUT); // gpio 15 as output
}

void loop() 
{
  for (int pwm {0}; pwm < 256; ++pwm) // loop for increasing the pwm value from 0 to 255 in 1 increments
  {
    analogWrite(ledpin, pwm); // output the duty cycle 0 is 0%, 255 is 100%
    delay(4); // delay of 4ms
  }

  for (int pwm {255}; pwm >= 0; --pwm)  // loop for decreasing the pwm value from 255 to 0 in 1 increments
  {
    analogWrite(ledpin, pwm);  // output the duty cycle 0 is 0%, 255 is 100%
    delay(4);  // delay of 4ms
  }
}
```

## Demo

The LED fades up over about a second, fades back down over about a second, and
repeats.

https://github.com/user-attachments/assets/e9ef990a-ec7d-4654-8220-ed35982c41a1


