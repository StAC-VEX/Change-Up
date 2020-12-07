#include "gameObjects.h"

/**
* This file is for the class that contains all of the main vex
* objects created and used in the code.
* 
* @author Bailey Moir
*/

namespace Syntech {
  competition GameObjects::Competition = competition();
  controller GameObjects::Controller = controller();

  motor GameObjects::leftMotor = motor(PORT10, false);
  motor GameObjects::rightMotor = motor(PORT1, true);
  motor GameObjects::intakeLeft = motor(PORT20, true);
  motor GameObjects::intakeRight = motor(PORT11, false);
  motor GameObjects::lift = motor(PORT16, true);

  inertial GameObjects::inertialSensor = inertial(PORT3);
}