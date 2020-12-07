#pragma once
#include "vex.h"
#include "gameObjects.h"

/**
* This file is for the class that contains the functions
* necessary to operate the autonomous.
*
* @author Bailey Moir
*/

namespace Syntech {
  class Auton {
  public:
    // The general speed of autonomous functions and motors.
    static constexpr float globalSpeed = 1000;

    /**
    * Turns the robot a certian amount of degrees.
    * @param degrees the degrees the robot will turn.
    * @param miliseconds how long the robot has to turn.
    */
    static void turnDeg( int degrees, double miliseconds );

    /**
    * Moves the robot a certain distance.
    * @param distance How far the robot will move. Measured in centimeters.
    * @param miliseconds how long the robot has to reach the end point.
    */
    static void move( float distance, double miliseconds );

    /**
    * Runs the intake
    * @param time how long the intake runs for.
    * @param speed the speed the intake runs at.
    */
    static void intake( float miliseconds, int speed );

  private:
    // The diameter of the back wheel(s) of the robot.
    static constexpr float wheelHeight = 10.4;
    
    // The circumference, diameter between the two back wheels.
    static constexpr float wheelCircumference = wheelHeight * 3.14159265359;
  };
}