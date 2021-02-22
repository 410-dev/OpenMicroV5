#include "main.h"

void motorRunMultiple(pros::Motor specifiedMotors[], int motorCounts, int speed) {
  for(int i = 0; i < motorCounts; i++) {
    specifiedMotors[i].move(speed);
  }
}

void motorRun(pros::Motor specifiedMotor, int speed) {
  specifiedMotor.move(speed);
}

void motorRunReverseMultiple(pros::Motor specifiedMotors[], int motorCounts, int speed) {
  for(int i = 0; i < motorCounts; i++) {
    specifiedMotors[i].move(-speed);
  }
}

void motorRunReverse(pros::Motor specifiedMotor, int speed) {
  specifiedMotor.move(-speed);
}

void motorStop(pros::Motor specifiedMotor) {
  specifiedMotor.move(0);
}

void motorStopMultiple(pros::Motor specifiedMotors[], int motorCounts) {
  for(int i = 0; i < motorCounts; i++) {
    specifiedMotors[i].move(0);
  }
}
