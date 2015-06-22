import processing.serial.*;
import org.firmata.*;
import cc.arduino.*;

Arduino arduino;


int motor_legPin = 9;

int h = 0;
boolean c = true;


void setup() {
  size(600, 400);
  background(255);
  
  //println(Arduino.list());
  arduino = new Arduino(this, Arduino.list()[7], 57600);
  arduino.pinMode(motor_legPin, Arduino.OUTPUT);
}

void draw() {
  int val = arduino.analogRead(0);
  float distance = map(val, 768, 255, 10, 80);
  println(distance);
  if(distance < 50){
  arduino.digitalWrite(motor_legPin, Arduino.HIGH);
  delay(100);
  arduino.digitalWrite(motor_legPin, Arduino.LOW);
  delay(100);
  }
}

