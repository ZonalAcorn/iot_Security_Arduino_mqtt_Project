This is a simple 3 arduino read and control system, controled entirely through an mqtt broker

**REQUIREMENTS**
* 3 Arduino R4 WiFi
* 1 mqtt broker (I used mosquitto on PI)
* 2 Arduino Sensor Kits (made with seeed studio)
* 1 MG90S Servo



**Normal operation**

The first Arduino (called R4-1 on the broker) monitors and publishes temp and humidity data, and controls a buzzer, both with the wired button, and from the broker.
The button turns on the buzzer for 2 seconds and then turns it off, the broker may send a 0 or 1 to iot/Control/Buzzer to toggle it on or off (0=off 1=on).

The second Arduino (called R4-2 on the broker) monitors light levels and a potentiometer (called "Dial"), it publishes this data to the broker, and uses the broker to control an LED.
The LED is turned on and off by sending a 0 or 1 to iot/Control/LED (0=off 1=on)

The third Arduino (called R4-3 on the broker) is completely dedicated to controlling a servo.
It is automatically controlled by the potentiometer from arduino 2, with no additional configuration required (assuming addressing is correct).
It can also be controlled from the broker by sending a number 0-180 to iot/Control/Servo_Speed, 89-0 goes in one direction and 91-180 goes in the other,
returning the motor to 90 (stopped) also returns the servo to automatic control (from the potentiometer).
