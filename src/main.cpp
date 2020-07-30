#include "vex.h"

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// drive                drivetrain    10, 1      
// leftMotor            motor         10    
// rightMotor           motor         1
// intakeLeft           motor         20
// intakeRight          motor         11   
// ---- END VEXCODE CONFIGURED DEVICES ----

using namespace vex;

//Motors
  motor leftMotor = motor(PORT10, false); //Swap ports
  motor rightMotor = motor(PORT1, true); //This part is being retarted and not changing when I change the bool

  motor miniL = motor(PORT9, true);
  motor miniR = motor(PORT2, false);

  drivetrain drive = drivetrain(leftMotor, rightMotor, 319.19, 120, 190, mm, 0.3333333333333333);

  motor intakeLeft = motor(PORT20, true);
  motor intakeRight = motor(PORT11, false);

  motor lift = motor(PORT16, true);
//

competition Competition;
controller Controller;

void pre_auton(void) {
  vexcodeInit();
}

//21.6812mm
float wheelSpan = 21.6812; //cm
float spanCircumference = wheelSpan * 3.14159265359; //cm  68.113
float wheelWidth = 10.16; //cm
float widthCircumference = wheelWidth * 3.14159265359; //cm  31.918
float ratio = spanCircumference / widthCircumference; //Converts wheel rotations to robot rotations when *
//Ratio is 2.133

//180 * wheelSpan * 3.14159265359

//Create individual PIDs for each motor.
//Figure out difference between leftMotor speed and rightMotor
void turnDeg( int degrees, double miliseconds ) {
  double startTime = vexSystemTimeGet(); //verified ms
  leftMotor.resetRotation();
  rightMotor.resetRotation();

  Controller.Screen.clearScreen();

  float Kp = 0.3;
  float leeway = 2;
  while (vexSystemTimeGet() < startTime + miliseconds &&
    !(leftMotor.rotation(deg) < degrees * ratio * 2 + leeway &&
    leftMotor.rotation(deg) > degrees * ratio * 2 - leeway &
    rightMotor.rotation(deg) < -degrees * ratio * 2 + leeway &&
    rightMotor.rotation(deg) > -degrees * ratio * 2 - leeway)) {
    double errorL = degrees * ratio * 2 - leftMotor.rotation(deg);
    double errorR = degrees * ratio * 2 + rightMotor.rotation(deg);

    double difference = errorL - errorR; //+ = r+; - = l+

    //180 = 200

    if (errorL > 112) errorL = 112;
    if (errorL < -112) errorL = -112;

    if (errorR > 112) errorR = 112;
    if (errorR < -112) errorR = -112;

    leftMotor.spin(directionType::fwd, errorL * Kp + difference / 2, rpm);
    rightMotor.spin(directionType::rev, errorR * Kp - difference / 2, rpm);
  }
}
void move( int distance, double miliseconds ) {
  double startTime = vexSystemTimeGet(); //verified ms
  float startDeg = leftMotor.rotation(deg);

  float Kp = 0.2;
  while (vexSystemTimeGet() < startTime + miliseconds && !(leftMotor.rotation(deg) - startDeg < distance / widthCircumference * 360 * 2 + 2 && leftMotor.rotation(deg) - startDeg > distance / widthCircumference * 360* 2 - 2)) {
    double error = distance / widthCircumference * 360 * 2 - (leftMotor.rotation(deg) - startDeg);
    double speed = error*Kp; //K is the constant

    if (speed > 112) speed = 112;
    if (speed < -112) speed = -112;

    leftMotor.spin(directionType::fwd, speed, pct);
    rightMotor.spin(directionType::fwd, speed, pct);
  }
}
void intake( float time, int speed) {
  intakeLeft.spin(fwd, speed, rpm);
  intakeRight.spin(fwd, speed, rpm);
  lift.spin(fwd, speed, rpm);

  wait(time, msec);

  intakeLeft.stop(coast);
  intakeRight.stop(coast);
  lift.stop(coast);
}

/*
    Instructions
      N: Start facing away from wall next to corner tower
      Y: Start facing mid ready to score
    
*/
int gss = 120; //global standard speed
void autonomous(void) {
  switch (0) {
    case 999:
    default:
      break;
    case 0:
      turnDeg(180, 5000);
      break;
    case 1: //BLY / RLY
      //10.5s
      intake(5000, gss);
      move(-10, 500);
      turnDeg(90, 5000);
      move(30, 300);
      turnDeg(90, 5000);
      move(110, 5000);
      intake(5000, gss);
      turnDeg(45, 5000);
      intake(5000, gss);
      break;
    case 2: //BLN / RLN
      /*
        move forward 30cm
        turn left 90
        move forward 30cm
        intake
        turn left 45
        move forward 10cm
        score
        move back 10cm
        turn right 135
        move forward 60cm
        turn left 90
        move forward 15
        turn right 90
        move forward 45
        score
      */
      move(30, 5000);
      turnDeg(-90, 5000);
      move(30, 5000);
      intake(5000, gss);
      turnDeg(-45, 5000);
      move(10, 5000);
      intake(5000, gss);
      move(-10, 5000);
      turnDeg(135, 5000);
      move(60, 5000);
      turnDeg(-90, 5000);
      move(15, 5000);
      turnDeg(90, 5000);
      move(45, 5000);
      intake(5000, gss);
      break;
    case 3: //BRY / RRY
      /*
        score
        move back 10cm
        turn -90
        move forward 30cm
        turn -90
        move forward 110cm
        intake
        turn -45
        score
      */
      //10.5s
      intake(1500, gss);
      move(-10, 250);
      turnDeg(-90, 1500);
      move(30, 300);
      turnDeg(-90, 1500);
      move(110, 1500);
      intake(1500, gss);
      turnDeg(-45, 500);
      intake(1500, gss);
      break;
    case 4: //BRN / RRN
      /*
        move forward 30cm
        turn right 90
        move forward 30cm
        intake
        turn right 45
        move forward 10cm
        score
        move back 10cm
        turn left 135
        move forward 60cm
        turn right 90
        move forward 15
        turn left 90
        move forward 45
        score
      */
      move(30, 5000);
      turnDeg(90, 5000);
      move(30, 5000);
      intake(5000, gss);
      turnDeg(45, 5000);
      move(10, 5000);
      intake(5000, gss);
      move(-10, 5000);
      turnDeg(-135, 5000);
      move(60, 5000);
      turnDeg(90, 5000);
      move(15, 5000);
      turnDeg(-90, 5000);
      move(45, 5000);
      intake(5000, gss);
      break;
  }
}

float speed = 2;
void speedUp() {
  speed = 2;
}
void speedDown() {
  speed = 0.5;
}

bool moveB = false;
void changeMove() {
  moveB = !moveB;
}

void usercontrol(void) {
  //autonomous();
  while (true) {
    //Tank control movement.
    if (!moveB) {
      leftMotor.spin(directionType::fwd, speed * Controller.Axis3.value(), velocityUnits::rpm);
      rightMotor.spin(directionType::fwd, speed * Controller.Axis2.value(), velocityUnits::rpm);
    } else {
      leftMotor.spin(directionType::fwd, speed * (Controller.Axis3.value() + Controller.Axis4.value()) / 2, velocityUnits::rpm);
      rightMotor.spin(directionType::fwd, speed * (Controller.Axis3.value() - Controller.Axis4.value()) / 2, velocityUnits::rpm);
    }

    //Controls the intake.
    intakeLeft.spin(directionType::fwd, 240 * (Controller.ButtonL1.pressing() - Controller.ButtonL2.pressing()), velocityUnits::rpm);
    intakeRight.spin(directionType::fwd, 240 * (Controller.ButtonL1.pressing() - Controller.ButtonL2.pressing()), velocityUnits::rpm);
    lift.spin(directionType::fwd, 50000 * (Controller.ButtonL1.pressing() - Controller.ButtonL2.pressing()), velocityUnits::rpm);

    //Runs the autonomous when you click the A button.
    Controller.ButtonA.pressed(autonomous);

    //Set the speed using presets.
    Controller.ButtonX.pressed(speedUp);
    Controller.ButtonB.pressed(speedDown);

    Controller.ButtonY.pressed(changeMove);

    miniL.spin(fwd, (240  * ((int)Controller.ButtonUp.pressing() - (int)Controller.ButtonDown.pressing()) / 2 - 240 * ((int)Controller.ButtonRight.pressing() - (int)Controller.ButtonLeft.pressing()) / 2), pct);
    miniR.spin(fwd, (240 * ((int)Controller.ButtonUp.pressing() - (int)Controller.ButtonDown.pressing()) / 2 + 240 * ((int)Controller.ButtonRight.pressing() - (int)Controller.ButtonLeft.pressing()) / 2), pct);
    
    wait(20, msec);
  }
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    wait(100, msec);
  }
}
