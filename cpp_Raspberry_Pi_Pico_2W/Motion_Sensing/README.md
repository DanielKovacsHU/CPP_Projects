# PIR motion detection with a Raspberry Pi Pico 2 W

An HC-SR501 PIR motion sensor on GP15 is read by a Raspberry Pi Pico 2 W. When the sensor output goes HIGH, the sketch prints that motion started. When it goes LOW, the sketch prints that motion ended and how long the output stayed HIGH. The sketch runs in the Arduino IDE. On the Pico 2 W the serial port is not a hardware UART; it is the USB CDC port, which creates a virtual COM port over the USB connection, so no GPIO pins are used for it.

## Components and design

### Parts

- Raspberry Pi Pico 2 W
- HC-SR501 PIR sensor module
- Breadboard
- Jumper wires
- USB cable (powered from a USB 3.0 port, 5 V / 900 mA)

### Electrical parameters

| Symbol | Parameter | Value |
| --- | --- | --- |
| U_supply | Supply voltage (module) | 5 V typical, 4.5 V to 12 V (some docs say 20 V) |
| U_out high | Output when triggered | 3.3 V |
| U_out low | Output when idle | 0 V |
| I_static | Static (quiescent) current | 50 uA |
| I_operating | Operating current | 65 mA |
| Range | Maximum sensing range | 7 m, under 120 degrees |
| Pin mode | Input mode | INPUT (the module drives the line actively) |

### HC-SR501 module

The module has three pins and two potentiometers.

| Module pin | Function |
| --- | --- |
| VCC | Power, 5 V |
| OUT | Digital output, 3.3 V when triggered, 0 V when idle |
| GND | 0 V |

| Potentiometer | Function |
| --- | --- |
| A | Delay (output HIGH time), 5 sec to 5 min, clockwise increases the delay |
| B | Sensing distance, 3 m to 7 m, clockwise increases the distance |

### Operating modes

The module has a mode selector, normally a simple 2 position switch soldered on the board.

- **H, repeat trigger enabled:** The output stays HIGH as long as motion is continuously detected. Every new detection resets the timer, so the output stays active until the person leaves and the delay time expires.
- **L, repeat trigger disabled:** The output goes HIGH for a fixed time (set by potentiometer A) and then drops to LOW, even if the motion continues. After that there is a short blocking time (about 2.5 sec) before the sensor can trigger again. This mode is useful for counting events or for a single pulse, instead of a continuous signal.

The switch is missing on my module, so it runs in the default H mode.

### Image of the moduls underside
<img width="290" height="353" alt="brave_thtEHFf5gX" src="https://github.com/user-attachments/assets/e9193c84-aa75-4423-8552-cd8cbfa6b60f" />


### Powering the sensor

The sensor needs 5 V, but the Pico 2 W can only output 3.3 V. The solution is to take the 5 V directly from the VBUS pin (pin 40), without touching the microcontroller. VBUS is the 5 V input from the micro-USB port, and it is fed through a Schottky diode to generate VSYS.

### The powerchain of the Pico 2 W board.
<img width="551" height="361" alt="brave_FGRZqdKudg" src="https://github.com/user-attachments/assets/c08b01f3-adaa-473c-88d9-c26476b9cd0a" />


The Pico 2 W is powered from a USB 3.0 port, which provides 5 V and 900 mA. There is little or no information about the maximum current the Pico 2 W can pull while using its wireless features and the 300 mA output limit together. In the Raspberry Pi forum, a user measured about 300 mA while using BLE. With the 65 mA sensor, the 900 mA port leaves enough margin.

The module is not meant to run from 3.3 V. At that supply it is documented to retrigger continuously regardless of motion.

### Behavior and tolerances

The module is built around a BISS0001 controller. Its documentation, and the vendors' documentation, disagree on many values, and the parts are not binned. So most values here are typical and should not be treated as a specification.

| Item | What is documented |
| --- | --- |
| Timer constant (Tx) | Differs between datasheet revisions (24576, 49152, 67025), so the same formula gives different results |
| Internal time base | 15 kHz to 17 kHz, and all durations carry this tolerance |
| Minimum delay (pot A) | 0.3 s, 1.2 s, 2.5 s, 3 s or 5 s, depending on the vendor |
| Lock time (Ti) | 0.2 s on some board revisions, 3 s on others |
| Comparator thresholds | Vt+ 1.3 V to 2.1 V, Vt- 0.6 V to 1.1 V, trigger point differs between units |
| Sensing element | RE200B, LHI778, PIR325, KP500B or KP506B, not marked on the module |
| Supply range | 4.5 V to 20 V |

- **Potentiometers:** They are plain trimmers with no detent and no marking. The position is not repeatable, and a small rotation changes the delay.
- **Lock time:** It decides whether a second trigger extends the HIGH period or is ignored. The blocking time can be set from zero to a few tens of seconds. After each output (HIGH to LOW) the sensor ignores all signals for that time, which also suppresses interference from load switching. In my tests its around 1.8 to 2 sec.
- **Temperature:** Above 30 to 32 degrees C the detection distance gets shorter. The RT pad for temperature compensation is unpopulated by default, and there is no specification for how the delay drifts with temperature.
- **Afterglow:** At the minimum delay, the warmed sensing element can trigger a second pulse after the body has already passed. It can be seen in my Demo
- **Lens:** The Fresnel lens divides the field into multiple zones, so one pass can produce more than one trigger.
- **Warm-up:** It takes 30 to 60 sec and can produce 0 to 3 false outputs. In my tests the boot-up time before the sensor settled was 2.6 to 7.7 sec in the first tests, and up to 16 sec after a long power-off.

### Schematic
<img width="436" height="359" alt="brave_r2N1aCQGgY" src="https://github.com/user-attachments/assets/7402c38c-ed2d-454f-9a0f-1e9dac3865e2" />

## Circuit and code

The sensor VCC connects to VBUS, the sensor OUT connects to GP15, and the sensor GND connects to GND.

| Pico 2 W pin | Connects to |
| --- | --- |
| 40 (VBUS) | Sensor VCC |
| 20 (GP15) | Sensor OUT |
| 38 (GND) | Sensor GND |

### Image of the circuit:
<img width="800" height="602" alt="motion_sense" src="https://github.com/user-attachments/assets/67604d85-9de8-4512-8c94-54eab3721ef1" />


### The code:

`last_state` holds the last accepted level as a `std::string`, either "low" (not triggered) or "high" (triggered). When the pin reads HIGH and the last state is "low", the start time is saved from `millis()`, the state is set to "high" and a start message is printed. When the pin reads LOW and the last state is "high", the state is set back to "low" and the HIGH time is printed in seconds. Checking `last_state` makes sure each trigger prints only one start and one end message, no matter how long the output stays HIGH. The printed time includes the hold delay of the module, because the output stays HIGH after the motion has stopped.

```cpp
#include <string>   // to use std::string type

const int readpin {15};   // readpin variable with value 15

void setup() 
{
  pinMode(readpin, INPUT);  // set pin 15 to input mode
  Serial.begin(1);  // begin serial communication
}

std::string last_state {"low"};   // start with a low state (not triggered)
unsigned long detection_start {};   // value initization of the variable

void loop() 
{
  if (digitalRead(readpin) == HIGH && last_state == "low")    // if currently there is a trigger (high value) and the last state is "low"
  {
    detection_start = millis();   // set the detection_start variable to the current time
    last_state = "high";    //  set the last_state variable to "high"
    Serial.println("Motion is detection started");    //  print out the message
  }

  if (digitalRead(readpin) == LOW && last_state == "high")    // if currently there is no trigger (low value read) and the last state is "high"
  {
    last_state = "low";   // set last_state to "low"
    Serial.println("Motion is detection ended after " + String((millis() - detection_start) / 1000) + "sec");   // print out the high signal time (how long was the movement), including the value hold delay
  }
}
```

## Measured hold time

Measured with millis() logs, A potentiometer at its minimum and the module in H mode, the output stayed HIGH for about 902 ms after a trigger. Most of the readings were accurate:

### Image of serial monitor logs
<img width="301" height="799" alt="Arduino_IDE_7z13D3fSo3" src="https://github.com/user-attachments/assets/e532b165-e612-4463-994e-12d4cba45a32" />



The longer values for example 1958 ms and 1888 ms are cases where the sensor was triggered again while the output was still HIGH, which extends the HIGH time.

The 902 ms was measured at one potentiometer angle and one room temperature. If the potentiometer position or the room temperature changes, the value cannot be assumed to be correct.

## Known issue: false positives

The sensor reports a lot of false motion. The cause that forum users discuss is most likely electrical noise on the PIR power line, which the sensor reads as a trigger. Almost any other device can cause it, and so can the microcontroller itself. The same problem appears when the MCU goes into sleep mode. Other documented sources are Wi-Fi/RF (the analog front end has no shielding), air currents and vibration.

A fix reported by other users, which I have not tested yet, is a small electrical shield over the back of the sensor. A 3D printed cover is covered with copper tape, and the copper is connected to ground.

## Demo

https://github.com/user-attachments/assets/37d3a869-2ef0-4f39-a972-ef4651f7ea8d

The demo showcases a false trigger before the long motion part.
