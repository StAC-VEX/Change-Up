#include "auton.h"

/**
* This file is for the class that contains the functions
* necessary to operate the autonomous.
*
* @author Bailey Moir
*/

namespace Syntech {
  /**
  * Turns the robot a certian amount of degrees.
  * @param degrees the degrees the robot will turn.
  * @param miliseconds how long the robot has to turn.
  */
  void Auton::turnDeg( int degrees, double miliseconds) {
    double startTime = vexSystemTimeGet(); //verified ms
    GameObjects::inertialSensor.resetRotation();
    GameObjects::leftMotor.resetRotation();
    GameObjects::rightMotor.resetRotation();

    float Kp = 1, margin = 2;
    while (vexSystemTimeGet() < startTime + miliseconds && (GameObjects::inertialSensor.rotation() > degrees + margin || GameObjects::inertialSensor.rotation() < degrees - margin)) {
      // How much the rotation needs to change.
      double error = degrees - GameObjects::inertialSensor.rotation(deg);
      // Accounts for the difference between the two motors and where they are supposed to be.
      double difference = (GameObjects::leftMotor.rotation(deg) + GameObjects::rightMotor.rotation(deg));
      // Caps the error.
      if (error > 60) error = 60;
      if (error < -60) error = -60;

      GameObjects::leftMotor.spin(directionType::fwd, (error - difference) * Kp, rpm);
      GameObjects::rightMotor.spin(directionType::rev, (error + difference) * Kp, rpm);
    }
    GameObjects::leftMotor.stop(hold);
    GameObjects::rightMotor.stop(hold);
  }

  /**
  * Moves the robot a certain distance.
  * @param distance How far the robot will move. Measured in centimeters.
  * @param miliseconds how long the robot has to reach the end point.
  */
  void Auton::move( float distance, double miliseconds ) {
    double startTime = vexSystemTimeGet(); // verified ms
    GameObjects::leftMotor.resetRotation();
    GameObjects::rightMotor.resetRotation();

    float Kp = 1, margin = 1;
    while (vexSystemTimeGet() < startTime + miliseconds && !(GameObjects::leftMotor.rotation(deg) < distance / wheelCircumference * 360 + margin && GameObjects::leftMotor.rotation(deg) > distance / wheelCircumference * 360 - margin)) {
      // How far the motor still needs to go.
      double error = distance / wheelCircumference * 360 - GameObjects::leftMotor.rotation(deg);
      // Caps the error.
      if (error > 60) error = 60;
      if (error < -60) error = -60;

      GameObjects::leftMotor.spin(directionType::fwd, error * Kp, rpm);
      GameObjects::rightMotor.spin(directionType::fwd, error * Kp, rpm);
    }
    GameObjects::leftMotor.stop(hold);
    GameObjects::rightMotor.stop(hold);
  }
  
  /**
  * Runs the intake
  * @param miliseconds how long the intake runs for.
  * @param speed the speed the intake runs at.
  */
  void Auton::intake( float miliseconds, int speed ) {
    float start = vexSystemTimeGet();
    // While it hasn't been [miliseconds], run the intake.
    while (vexSystemTimeGet() - start < miliseconds) {
      GameObjects::intakeLeft.spin(directionType::rev, speed, rpm);
      GameObjects::intakeRight.spin(directionType::rev, speed, rpm);
      GameObjects::lift.spin(directionType::rev, speed, rpm);
    }

    // Stop the intake.
    GameObjects::intakeLeft.stop(coast);
    GameObjects::intakeRight.stop(coast);
    GameObjects::lift.stop(coast);
  }
}