#include <Servo.h>
#include <Ramp.h>

// define all servo objects
Servo baseServo;
Servo shoulderServo;  
Servo elbowServo;     
Servo wristRotServo;  
Servo wristTiltServo; 

// interpolation setup
rampInt baseRamp;
rampInt shoulderRamp;
rampInt elbowRamp;
rampInt wristRotRamp;
rampInt wristTiltRamp;

// interpolation settings
const unsigned long waveCycleTime = 8000;
const unsigned long smallMotionTime = 3000;

void setup() {
  // attach all servos to their pins
  baseServo.attach(9);
  shoulderServo.attach(10);
  elbowServo.attach(11);
  wristRotServo.attach(7);
  wristTiltServo.attach(6);

  // set position
  baseServo.write(90);
  shoulderServo.write(90);
  elbowServo.write(90);
  wristRotServo.write(90);
  wristTiltServo.write(90);
  
  // base - main rotation
  baseRamp.go(120, waveCycleTime, CUBIC_INOUT, BACKANDFORTH);
  baseRamp.setGrain(10);
  
  // shoulder - big up/down motion
  shoulderRamp.go(60, waveCycleTime*1.5, QUADRATIC_INOUT, BACKANDFORTH);
  shoulderRamp.setGrain(10);
  
  // elbow - complementary motion
  elbowRamp.go(120, waveCycleTime*0.8, CUBIC_INOUT, BACKANDFORTH);
  elbowRamp.setGrain(10);
  
  // wrist rotation - side to side
  wristRotRamp.go(60, smallMotionTime, QUADRATIC_INOUT, BACKANDFORTH);
  wristRotRamp.setGrain(10);
  
  // wrist flexion - up/down
  wristTiltRamp.go(60, smallMotionTime*1.3, CUBIC_INOUT, BACKANDFORTH);
  wristTiltRamp.setGrain(10);

  delay(2000);
}

void loop() {
  // update all ramps
  baseRamp.update();
  shoulderRamp.update();
  elbowRamp.update();
  wristRotRamp.update();
  wristTiltRamp.update();

  // apply interpolation
  baseServo.write(baseRamp.getValue());
  shoulderServo.write(shoulderRamp.getValue());
  elbowServo.write(elbowRamp.getValue());
  wristRotServo.write(wristRotRamp.getValue());
  wristTiltServo.write(wristTiltRamp.getValue());

  delay(15); // delay
}
