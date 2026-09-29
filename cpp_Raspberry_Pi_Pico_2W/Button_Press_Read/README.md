Because Serial on the Pico 2W is not a hardware UART — it's the USB CDC (Communication Device Class) port.  It creates a virtual COM port directly over the USB connection to your computer, so no GPIO pins are involved at all. [source: Arduino](https://docs.arduino.cc/language-reference/en/functions/communication/serial/begin/#:~:text=For%20USB%20CDC,with%20these%20ports)

For USB CDC serial ports (e.g. Serial on the Leonardo), Serial.begin() is irrelevant. You can use any baud rate and configuration for serial communication with these ports.

println :
Prints data to the serial port as human-readable ASCII text followed by a carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n')

------------------------------------------------------------------------------------------------------------------------------
# Only relevant if the input is not a stable 3.3V high value: 
[e9 issue](https://pip-assets.raspberrypi.com/categories/1214-rp2350/documents/RP-008373-DS-2-rp2350-datasheet.pdf#page=1367&zoom=100,153,374)


