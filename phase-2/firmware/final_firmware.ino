#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

const int servoPins[] = {8, 9, 10, 11, 12, 13};
const int numServos = sizeof(servoPins)/sizeof(servoPins[0]);
int currentPositions[numServos] = {90, 90, 90, 90, 90, 90};

void setup() {
  Serial.begin(115200);
  
  for(int i = 0; i < numServos; i++) {
    switch(i) {
      case 0: servo1.attach(servoPins[i]); break;
      case 1: servo2.attach(servoPins[i]); break;
      case 2: servo3.attach(servoPins[i]); break;
      case 3: servo4.attach(servoPins[i]); break;
      case 4: servo5.attach(servoPins[i]); break;
      case 5: servo6.attach(servoPins[i]); break;
    }
    switch(i) {
      case 0: servo1.write(currentPositions[i]); break;
      case 1: servo2.write(currentPositions[i]); break;
      case 2: servo3.write(currentPositions[i]); break;
      case 3: servo4.write(currentPositions[i]); break;
      case 4: servo5.write(currentPositions[i]); break;
      case 5: servo6.write(currentPositions[i]); break;
    }
  }
}

void loop() {
  if(Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    int servoNum = input.substring(0, input.indexOf(',')).toInt();
    int position = input.substring(input.indexOf(',') + 1).toInt();
    
    if(servoNum >= 1 && servoNum <= numServos && position >= 0 && position <= 180) {
      currentPositions[servoNum-1] = position;
      
      switch(servoNum-1) {
        case 0: servo1.write(position); break;
        case 1: servo2.write(position); break;
        case 2: servo3.write(position); break;
        case 3: servo4.write(position); break;
        case 4: servo5.write(position); break;
        case 5: servo6.write(position); break;
      }
      
      Serial.print("Moved servo ");
      Serial.print(servoNum);
      Serial.print(" to ");
      Serial.println(position);
    }
  }
}
