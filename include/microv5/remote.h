#include "main.h"

AEMotorControl mtctl;
AESensorsUtility srutil;

  bool actionUpdate = false;
  bool liveControl = false;
  float multiplier = 1.0;

  string recentActivity = "";
  string FWD = "";
  string LFT = "";

  float speed = 25.0;

	void updateAll(controller Controller) {
    if (!liveControl) {
      encLeft.at(encLeft.size() - 1) = srutil.getShaftEncoderValue(ENCODER_LEFT);
      encRight.at(encRight.size() - 1) = srutil.getShaftEncoderValue(ENCODER_RIGHT);
      encSide.at(encSide.size() - 1) = srutil.getShaftEncoderValue(ENCODER_SIDE);
    }

    FWD = toString(Controller.Axis3.value());
    LFT = toString(Controller.Axis4.value());

    multiplier = 1.0;
    if (Controller.ButtonL1.pressing()) multiplier = 2.0;
    if (Controller.ButtonL2.pressing()) multiplier = 4.0;

    if (Controller.ButtonUp.pressing() || Controller.Axis3.value() > 120) onPress_forward();
    else if (Controller.ButtonDown.pressing() || Controller.Axis3.value() < -120) onPress_backward();
    else if (Controller.ButtonLeft.pressing() || Controller.Axis4.value() < -120 || Controller.Axis1.value() < -120) onPress_turnRight();
    else if (Controller.ButtonRight.pressing() || Controller.Axis4.value() > 120 || Controller.Axis1.value() > 120) onPress_turnLeft();
    else if (Controller.ButtonX.pressing() || Controller.Axis2.value() > 120) onPress_startPuller();
    else{
      onRelease_wheels();
      onRelease_puller();
    }
    if (Controller.ButtonA.pressing()) onPress_systemTerminate();
  }

  void addIndex() {
    encLeft.push_back(0);
    encRight.push_back(0);
    encSide.push_back(0);
    srutil.resetShaftEncoderValue(ENCODER_LEFT);
    srutil.resetShaftEncoderValue(ENCODER_RIGHT);
    srutil.resetShaftEncoderValue(ENCODER_SIDE);
    actionUpdate = false;
  }

  void onRelease_wheels() {
    if(actionUpdate && !liveControl) addIndex();
    recentActivity = "REMOTE: RELEASED";
    motor allmotors[] = {WHEEL_FRONT_LEFT,WHEEL_BACK_RIGHT,WHEEL_BACK_LEFT,WHEEL_FRONT_RIGHT};
    mtctl.stopMotors(allmotors, 4);
  }

  void onPress_forward() {
    if(!actionUpdate) actionUpdate = true;
    recentActivity = "REMOTE: FORWARD";
    motor allmotors[] = {WHEEL_FRONT_LEFT,WHEEL_BACK_RIGHT,WHEEL_BACK_LEFT,WHEEL_FRONT_RIGHT};
    mtctl.runMotors(allmotors, 4, directionType::rev, speed*multiplier);
  }

  void onPress_backward() {
    if(!actionUpdate) actionUpdate = true;
    recentActivity = "REMOTE: BACKWARD";
    motor allmotors[] = {WHEEL_FRONT_LEFT,WHEEL_BACK_RIGHT,WHEEL_BACK_LEFT,WHEEL_FRONT_RIGHT};
    mtctl.runMotors(allmotors, 4, directionType::fwd, speed*multiplier);
  }

  void onPress_turnLeft() {
    if(!actionUpdate) actionUpdate = true;
    recentActivity = "REMOTE: LEFT";
    motor toRunForward[] = {WHEEL_FRONT_RIGHT, WHEEL_BACK_RIGHT};
    motor toRunBackward[] = {WHEEL_FRONT_LEFT, WHEEL_BACK_LEFT};
    mtctl.runMotors(toRunForward, 2, directionType::fwd, speed*multiplier);
    mtctl.runMotors(toRunBackward, 2, directionType::rev, speed*multiplier);
  }

  void onPress_turnRight() {
    if(!actionUpdate) actionUpdate = true;
    recentActivity = "REMOTE: RIGHT";
    motor toRunBackward[] = {WHEEL_FRONT_RIGHT, WHEEL_BACK_RIGHT};
    motor toRunForward[] = {WHEEL_FRONT_LEFT, WHEEL_BACK_LEFT};
    mtctl.runMotors(toRunForward, 2, directionType::fwd, speed*multiplier);
    mtctl.runMotors(toRunBackward, 2, directionType::rev, speed*multiplier);
  }

  void onPress_startPuller() {
    if(!actionUpdate) actionUpdate = true;
    recentActivity = "REMOTE: PULL";
    motor pullers[] = {PULL_MOTOR_1, PULL_MOTOR_2, INTAKE_LEFT, INTAKE_RIGHT};
    mtctl.runMotors(pullers, 4, directionType::fwd, speed*multiplier);
  }

  void onRelease_puller() {
    motor pullers[] = {PULL_MOTOR_1, PULL_MOTOR_2, INTAKE_LEFT, INTAKE_RIGHT};
    mtctl.stopMotors(pullers, 4);
  }

  void onPress_systemTerminate() {
    recentActivity = "REMOTE: STOP";
    mtctl.stopMotors(MOTOR_ALL, 8);
  }
