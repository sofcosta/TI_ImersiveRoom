/**
Capacitive Sensor and servo motor
Quando existe contacto com o capacitive sensor é acionado o motor servo.
baseado no codigo de: Sérgio M. Rebelo, Ana Cláudia Rodrigues, and Tiago Cruz
Tecnologias de Interface, 2023
Alexandra Coelho
Carolina Malta
Cátia Pereira
Sofia Costa
*/

#include <CapacitiveSensor.h>

#include <Servo.h>

// 10M resistor between pins 7 & 6, // pin 6 is sensor pin, add a wire and or foil if desired

CapacitiveSensor   cs_7_6 = CapacitiveSensor(7, 6);
Servo servo;

const int SERVO_PIN  = 3; // Arduino pin connected to servo motor's pin

int angle = 0;
int inc = 10;

void setup() {
  Serial.begin(9600);

  // turn off autocalibrate on channel 1 - just as an example
  cs_7_6.set_CS_AutocaL_Millis(0xFFFFFFFF);

  servo.attach(SERVO_PIN);
  servo.write(angle);
}

void loop() {
  long start = millis();
  long total1 =  cs_7_6.capacitiveSensor(30);
  
  if (total1 > 5000) {
    if (angle + inc > 180)inc = -10;
    else if (angle + inc < 0) inc = 10;
    angle += inc;
  }

  Serial.println(total1);
  delay(10);
  servo.write(angle);
}
