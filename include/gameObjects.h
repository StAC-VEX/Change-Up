#pragma once
#include "vex.h"

/**
* This file is for the class that contains all of the main vex
* objects created and used in the code.
*
* @author Bailey Moir
*/

namespace Syntech {
  class GameObjects {
  public:
    static competition Competition;
    static controller Controller;

    static motor leftMotor;
    static motor rightMotor;
    static motor intakeLeft;
    static motor intakeRight;
    static motor lift;

    static inertial inertialSensor;
  };
}