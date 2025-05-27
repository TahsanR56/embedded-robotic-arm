#include <Servo.h>

#define NUM_SERVOS 4

Servo servos[NUM_SERVOS];

const int potPins[NUM_SERVOS] = {A0, A1, A2, A3};  
const int servoPins[NUM_SERVOS] = {3, 5, 6, 9};     

int potValues[NUM_SERVOS];
int prevPotValues[NUM_SERVOS];

void setup() {
  Serial.begin(9600);  
  
  
  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].attach(servoPins[i]);
    pinMode(potPins[i], INPUT);  
  }
  
  
  for (int i = 0; i < NUM_SERVOS; i++) {
    potValues[i] = analogRead(potPins[i]);
    prevPotValues[i] = potValues[i];
    servos[i].write(map(potValues[i], 0, 1023, 0, 180));  // Initial servo position
  }
}

void loop() {
  
  for (int i = 0; i < NUM_SERVOS; i++) {
    potValues[i] = analogRead(potPins[i]);
    
    if (abs(potValues[i] - prevPotValues[i]) > 5) {
      int angle = map(potValues[i], 0, 1023, 0, 180);
      servos[i].write(angle);
      prevPotValues[i] = potValues[i];
      
      
      Serial.print("Servo ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(angle);
    }
  }
  
  delay(20);
}
