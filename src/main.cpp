#include "vex.h"

using namespace vex;

//Base variables
  competition Competition;
  controller Controller;

  motor leftMotor = motor(PORT10, false); //Swap ports
  motor rightMotor = motor(PORT1, true); //This part is being retarted and not changing when I change the bool
  motor intakeLeft = motor(PORT20, true);
  motor intakeRight = motor(PORT11, false);
  motor lift = motor(PORT16, true);

  motor miniL = motor(PORT9, true);
  motor miniR = motor(PORT2, false);

  inertial inertialSensor = inertial(PORT21);
//

class Vector3f {
  public:
    float x, y, z;

    Vector3f(float ax, float ay, float az) {
      x = ax;
      y = ay;
      z = az;  
    };
};

class Autonomous {
  public:
    float globalSpeed = 120;
    
    float wheelHeight = 10.16;
    float wheelCircumference = wheelHeight * 3.14159265359;
};
Autonomous auton = *new class Autonomous();

void pre_auton(void) {
  vexcodeInit();
  inertialSensor.calibrate();
}

void turnDeg( int degrees, double miliseconds ) {
  double startTime = vexSystemTimeGet(); //verified ms
  inertialSensor.resetRotation();
  leftMotor.resetRotation();
  rightMotor.resetRotation();

  float Kp = 0.7;
  float leeway = 1;
  waitUntil(!inertialSensor.isCalibrating());
  while (vexSystemTimeGet() < startTime + miliseconds && !(
    inertialSensor.rotation() < degrees + leeway &&
    inertialSensor.rotation() > degrees - leeway)) {
    
    double error = degrees - inertialSensor.rotation(deg);
    double difference = leftMotor.rotation(deg) + rightMotor.rotation(deg); //- = r+; + = l+ NO

    //Min
    if (error > 112) error = 112;
    if (error < -112) error = -112;
    //Max
    if (error < 24 && error > 0) error = 24;
    if (error > -24 && error < 0) error = -24;

    leftMotor.spin(directionType::fwd, error * Kp - difference, rpm);
    rightMotor.spin(directionType::rev, error * Kp + difference, rpm);
  }
}
void move( int distance, double miliseconds ) {
  double startTime = vexSystemTimeGet(); //verified ms
  float startDeg = leftMotor.rotation(deg);

  float Kp = 0.2;
  while (vexSystemTimeGet() < startTime + miliseconds && !(leftMotor.rotation(deg) - startDeg < distance / auton.wheelCircumference * 360 * 2 + 2 && leftMotor.rotation(deg) - startDeg > distance / auton.wheelCircumference * 360* 2 - 2)) {
    Vector3f accel = *new class Vector3f(inertialSensor.acceleration(axisType::xaxis), inertialSensor.acceleration(axisType::yaxis), inertialSensor.acceleration(axisType::zaxis));
    //Find out wtf * means
    double error = distance / auton.wheelCircumference * 360 * 2 - (leftMotor.rotation(deg) - startDeg);
    double speed = error*Kp; //Kp is the constant

    if (speed > 112) speed = 112;
    if (speed < -112) speed = -112;

    leftMotor.spin(directionType::fwd, speed - accel.y, pct); //Take away or plus one of the axis
    rightMotor.spin(directionType::fwd, speed - accel.y, pct); //Take away or plus one of the axis
  }
}
void intake( float time, int speed ) {
  float start = vexSystemTimeGet();
  while (start + vexSystemTimeGet() < start + time) {
    intakeLeft.spin(directionType::rev, speed, rpm);
    intakeRight.spin(directionType::rev, speed, rpm);
    lift.spin(directionType::rev, speed, rpm);
  }

  intakeLeft.stop(coast);
  intakeRight.stop(coast);
  lift.stop(coast);
}

/*
    Instructions
      N: Start facing away from wall next to corner tower
      Y: Start facing mid ready to score
    
*/
void autonomous(void) {
  switch (0) {
    case 999:
    default:
      break;
    case 777:
      intake(1000, auton.globalSpeed);
      intake(5000, auton.globalSpeed);
      break;
    case 0:
      turnDeg(180, 100000);
      break;
    case 1: //BLY / RLY
      //10.5s
      intake(5000, auton.globalSpeed);
      move(-10, 500);
      turnDeg(90, 5000);
      move(30, 300);
      turnDeg(90, 5000);
      move(110, 5000);
      intake(5000, auton.globalSpeed);
      turnDeg(45, 5000);
      intake(5000, auton.globalSpeed);
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
      intake(5000, auton.globalSpeed);
      turnDeg(-45, 5000);
      move(10, 5000);
      intake(5000, auton.globalSpeed);
      move(-10, 5000);
      turnDeg(135, 5000);
      move(60, 5000);
      turnDeg(-90, 5000);
      move(15, 5000);
      turnDeg(90, 5000);
      move(45, 5000);
      intake(5000, auton.globalSpeed);
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
      intake(1500, auton.globalSpeed);
      move(-10, 250);
      turnDeg(-90, 1500);
      move(30, 300);
      turnDeg(-90, 1500);
      move(110, 1500);
      intake(1500, auton.globalSpeed);
      turnDeg(-45, 500);
      intake(1500, auton.globalSpeed);
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
      intake(5000, auton.globalSpeed);
      turnDeg(45, 5000);
      move(10, 5000);
      intake(5000, auton.globalSpeed);
      move(-10, 5000);
      turnDeg(-135, 5000);
      move(60, 5000);
      turnDeg(90, 5000);
      move(15, 5000);
      turnDeg(-90, 5000);
      move(45, 5000);
      intake(5000, auton.globalSpeed);
      break;
  }
}

bool moveB = false;
void changeMove() {
  moveB = !moveB;
}

void usercontrol(void) {
  while (true) {
    if (!moveB) {
      leftMotor.spin(directionType::fwd, Controller.Axis3.value() * 2, velocityUnits::rpm);
      rightMotor.spin(directionType::fwd, Controller.Axis2.value() * 2, velocityUnits::rpm);
    } else {
      leftMotor.spin(directionType::fwd, Controller.Axis3.value() + Controller.Axis4.value(), velocityUnits::rpm);
      rightMotor.spin(directionType::fwd, Controller.Axis3.value() - Controller.Axis4.value(), velocityUnits::rpm);
    }

    float intake = 999 * (Controller.ButtonL1.pressing() - Controller.ButtonL2.pressing());
    intakeLeft.spin(directionType::fwd, intake, velocityUnits::rpm);
    intakeRight.spin(directionType::fwd, intake, velocityUnits::rpm);
    lift.spin(directionType::fwd, intake, velocityUnits::rpm);
    if (intake == 0) {
      lift.stop(hold);
    }
    
    Controller.ButtonA.pressed(autonomous);
    Controller.ButtonY.pressed(changeMove);

    miniL.spin(fwd, (240  * ((int)Controller.ButtonUp.pressing() - (int)Controller.ButtonDown.pressing()) / 2 - 240 * ((int)Controller.ButtonRight.pressing() - (int)Controller.ButtonLeft.pressing()) / 4), pct);
    miniR.spin(fwd, (240 * ((int)Controller.ButtonUp.pressing() - (int)Controller.ButtonDown.pressing()) / 2 + 240 * ((int)Controller.ButtonRight.pressing() - (int)Controller.ButtonLeft.pressing()) / 4), pct);
    
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