This is my RGB project :

calculation i used : rgb calculations :

* we adjust the led voltage 
* we adjust it by using 1 series resistor
* we assumes the voltage of maximum continuous current
* we choose the resistor to have the voltage or the led same or lower than the stated voltage of contionus current



red forward voltage U_f_red : 2V
blue forward voltage U_f_blue : 3V
green forward voltage U_f_blue : 3V
maximum forward current I_f : 20mA (must be same or below it)
pin voltage U_pin = 3.3V


R_series_red = U_series_red_resistor / I_f = (U_pin - U_f_red)/ I_f = (3.3-2)/0.02 = 65 ohm >> closes i have is 100ohm

as we chose a higher resistance than the original calculated value the circuit current will decline, which is equal to the resistance and led current (they connected in series). we dont know anything about the led at a lower current value, no resistance, no voltage, but can safely assume a minor forward voltage drop so i use a 10% voltage drop which is 1.8V.

With knowing the led voltage (rough value), the starting voltage and the series resistance value we can get the current on the resistor

I_R_series_red = U_resistor/R_series_red = 3.3-1.8/100 = 1.5/100 = 15mA rough current, which is below the 20mA threshold

for the blue/green we use the same method with different numbers

R_series_blue = U_series_blue_resistor / I_f = (U_pin - U_f_blue)/ I_f = (3.3-3)/0.02 = 15 ohm >> closes i have is 100ohm

I_R_series_blue = U_resistor/R_series_blue = 3.3-3/100 = 0.3/100 = 3mA rough current, which is very low compared to the maximum 20mA, will be much less visible



the values are same for the green led

R_series_green = U_series_green_resistor / I_f = (U_pin - U_f_green)/ I_f = (3.3-3)/0.02 = 15 ohm >> closes i have is 100ohm

I_R_series_green = U_resistor/R_series_green = 3.3-3/100 = 0.3/100 = 3mA rough current, which is also very low compared to the maximum 20mA, will be less visible
