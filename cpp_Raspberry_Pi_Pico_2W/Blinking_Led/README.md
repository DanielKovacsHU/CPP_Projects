# My first embedded project, using the Raspberry Pi Pico 2W to implement a simple led blinker

# Calculation i used :

- max continuous forward current I_led = 20mA, will use 15mA so we have safety headroom
- red led forward voltage at 15mA U_f = 2V (rough value)
- pico output U_pin = 3.3V

to get the resistance required to met the 15mA is following :

- 1, get the voltage drop in the resistor (U_pin - U_f)
- 2, know the current through it, which is 15mA
- 3, resistance by ohm's law the voltage drop of the component divided by the current through it so:

- R_series = (U_pin - U_f) / I_led = (3.3V - 2V) / 0.015A = 86.66ohm
- need to use the closest available alternative, so I will use a 100ohm resistor, which will gave around 13mA to the led, well below the maximum continous forward current


