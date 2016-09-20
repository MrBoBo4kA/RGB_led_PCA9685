# RGB_led_PCA9685
Using arduino for control five different RGB led via PCA9685

My first completed project on arduino.

Base - arduino Nano.

Im using module like this - https://www.adafruit.com/product/815
Need RGB LEDs with common anode and connect it on V+, cathodes connected to the ports PWM0-PWM14 on PCA9685 (LED0-LED14 on datasheets for PCA9685). R-led1 connected PWM0, G-led1 connected PWM1, B-led1 connected PWM2, R-led2 connected PWM3 etc.

OE port not connected  
SDA - A4 arduino  
SCL - A5 arduino  
Vcc and V+ connected +5v arduino  
all GND connect together  

Im using tilt sensor SW-520D and one pin from all connect to GND. Other sensor pin connect to port4-7 arduino. Sensors arranged in a square and inclined. In this way, when tilted one of the four sides, one sensor is triggered. Depending on which side is inclined will be regulated speed or brightness.

Power on any usb PC port or cellphone charger.
