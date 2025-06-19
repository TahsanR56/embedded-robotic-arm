#include <Servo.h>
#include <Ramp.h>

Servo gripper; 
rampInt gripperRamp;


const unsigned long smallCycleTime = 500; 

void setup() {
  gripper.attach(9);

  gripper.write(0);
  
  gripperRamp.go(75, smallCycleTime, CUBIC_INOUT, BACKANDFORTH);
  gripperRamp.setGrain(10); // Smoothness setting
  
  delay(2000);
}

void loop() {
  gripperRamp.update();

  gripper.write(gripperRamp.getValue());

  delay(15);
}
