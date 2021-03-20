#include "vex.h"
#include <sstream>
#include <cstdlib>
/**
* This file is the starting off point of the program.
*
* @author Bailey Moir
*/

/*- Global Variables -*/
  bool complete = false;
  float speed = 1;

  const float wheelHeight = 10.4;
  const float wheelCircumference = wheelHeight * 3.14159265359;
/*--------------------*/

/*---  Motor Setup ---*/
  competition Competition = competition();
  controller Controller = controller();

  motor leftMotor = motor(PORT4, false);
  motor rightMotor = motor(PORT9, true);
  motor leftFMotor = motor(PORT3, false);
  motor rightFMotor = motor(PORT1, true);
  motor intakeLeft = motor(PORT8, false);
  motor intakeRight = motor(PORT20, true);
  motor firstLift = motor(PORT2, true);
  motor secondLift = motor(PORT10, false); //Higher

  inertial inertialSensor = inertial(PORT11);
/*--------------------*/

/*- Auton Func & Cls -*/
  int consoleLine = 0;
  class Console { //480 x 240
  public:
    //Constructor
    static void init() { 
      Brain.Screen.clearScreen();
      Brain.Screen.setFillColor("#ff006f");
      Brain.Screen.drawRectangle(0, 0, 480, 240);
      Brain.Screen.setFillColor("#ffffff");
      Brain.Screen.drawRectangle(10, 10, 460, 220);
      //Brain.Screen.drawImageFromFile("Syntech.png", 0, 0);
    }

    /*static void print(const char* text) {
      if (consoleLine == 10) { //Over
        consoleLine = 0;
        init();
      }
      Brain.Screen.setFillColor("#ffffff");
      Brain.Screen.setPenColor("000000");
      Brain.Screen.setCursor(consoleLine + 2, 2);
      Brain.Screen.print(text);
      consoleLine++;
    }*/
    static void print(const char* text) {
      if (consoleLine == 10) { //Over
        consoleLine = 0;
        init();
      }
      Brain.Screen.setFillColor("#ffffff");
      Brain.Screen.setPenColor("000000");
      Brain.Screen.setCursor(consoleLine + 2, 2);
      Brain.Screen.print(text);
      consoleLine++;
    }

    static void print(double text) {
      if (consoleLine == 10) { //Over
        consoleLine = 0;
        init();
      }
      Brain.Screen.setFillColor("#ffffff");
      Brain.Screen.setPenColor("000000");
      Brain.Screen.setCursor(consoleLine + 2, 2);
      sstream ss;
      ss << text;
      Brain.Screen.print(ss.str().c_str());
      consoleLine++;
    }

    static void clear() {
      consoleLine = 0;
      Brain.Screen.setFillColor("#ffffff");
      Brain.Screen.setPenColor("000000");
      Brain.Screen.setCursor(consoleLine + 2, 2);
    }
  };
  void move( float distance, float multiplier ) {
    leftMotor.resetRotation();
    rightMotor.resetRotation();

    float margin = 1;
    double error = distance / wheelCircumference * 360 - leftMotor.rotation(deg);
    double previousError = 0;
    const double maxAcceleration = 0.03;
    while (!(leftMotor.rotation(deg) < distance / wheelCircumference * 360 + margin && leftMotor.rotation(deg) > distance / wheelCircumference * 360 - margin)) {
      // Gets the previous error
      error = distance / wheelCircumference * 360 - leftMotor.rotation(deg);
      double speed = error;

      /*if (fabs(speed - previousError) > maxAcceleration) {
        //If accelerating
        if (speed - previousError > 0) speed = previousError - maxAcceleration;
        else speed = previousError + maxAcceleration;
      }*/

      Console::print(speed);

      // Caps the error.
      if (speed > 60) speed = 60;
      if (speed < -60) speed = -60;
      if (speed < 5 && error > 0) speed = 5;
      if (speed > -5 && speed < 0) speed = -5;

      leftMotor.spin(directionType::fwd, error * multiplier, rpm);
      rightMotor.spin(directionType::fwd, error * multiplier, rpm);
      leftFMotor.spin(directionType::fwd, error * multiplier, rpm);
      rightFMotor.spin(directionType::fwd, error * multiplier, rpm);

      wait(20, msec);
      // How far the motor still needs to go.
      previousError = error;
    }
    leftMotor.stop(hold);
    rightMotor.stop(hold);
  }
  void moveTime( float miliseconds, float multiplier ) {
    leftMotor.spin(directionType::fwd, 127 * multiplier, rpm);
    rightMotor.spin(directionType::fwd, 127 * multiplier, rpm);
    leftFMotor.spin(directionType::fwd, 127 * multiplier, rpm);
    rightFMotor.spin(directionType::fwd, 127 * multiplier, rpm);
    
    wait(miliseconds, msec);

    leftMotor.stop(hold);
    rightMotor.stop(hold);
  }
/*--------------------*/

/*-- Gnrl Functions --*/
  void changeSpeed() {
    if (speed == 1) speed = 3;
    else speed = 1;
    Controller.Screen.clearScreen();
    Controller.Screen.setCursor(0, 0);
    Controller.Screen.print("Speed:");
    Controller.Screen.setCursor(2, 3);
    Controller.Screen.print(1 / speed);
  }
/*--------------------*/

/*---- Main Loops ----*/
  //See details.txt for details
  void autonomous(void) {    
    moveTime(1000, 1);
    firstLift.spin(fwd, 127, rpm);
    secondLift.spin(directionType::rev, 127, rpm);
    wait(5000, msec);
    firstLift.stop(hold);
    secondLift.stop(hold);
    moveTime(1000, -1);   

    complete = true;
  }
  void usercontrol() {
    leftMotor.stop(coast);
    rightMotor.stop(coast);

    Controller.ButtonA.pressed(changeSpeed);

    while (true) {
      // The difference between how fast the motors should  be going and the input.
      float diffL = (leftMotor.velocity(rpm) - Controller.Axis3.value()) * .12; // /.12 = / 60 / 360
      float diffR = (rightMotor.velocity(rpm) - Controller.Axis2.value()) * .12;
      
      if(Controller.Axis3.value() == 0) diffL = 0;
      if(Controller.Axis2.value() == 0) diffR = 0;
        
      leftMotor.spin(directionType::fwd, (Controller.Axis3.value() - diffL) * 2 / speed, velocityUnits::rpm);
      rightMotor.spin(directionType::fwd, (Controller.Axis2.value() - diffR) * 2 / speed, velocityUnits::rpm);
      leftFMotor.spin(directionType::fwd, (Controller.Axis3.value() - diffL) * 2 / speed, velocityUnits::rpm);
      rightFMotor.spin(directionType::fwd, (Controller.Axis2.value() - diffR) * 2 / speed, velocityUnits::rpm);

      float intakeTop = 999 * (Controller.ButtonL1.pressing() - Controller.ButtonL2.pressing());
      float intakeBtm = 999 * (Controller.ButtonR2.pressing() - Controller.ButtonR1.pressing());
      intakeLeft.spin(directionType::fwd, intakeBtm, velocityUnits::rpm);
      intakeRight.spin(directionType::fwd, intakeBtm, velocityUnits::rpm);
      firstLift.spin(directionType::fwd, intakeBtm, velocityUnits::rpm);
      secondLift.spin(directionType::fwd, intakeTop, velocityUnits::rpm);
      // Holds the lift
      if (intakeBtm == 0) firstLift.stop(hold);
      if(intakeTop == 0) secondLift.stop(hold);

      wait(20, msec);
    }    
  }
/*--------------------*/

int main() {
  vexcodeInit();
  Console::init();

  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  while (true) {
    wait(100, msec);
  }
}