#include "vex.h"
#include "sstream"
#include "console.h"
#include "auton.h"
#include "ui.h"
#include "vector2f.h"

/**
* This file is the starting off point of the program.
*
* @author Bailey Moir
*/

namespace Syntech {
  /*
  * 1 = left
  * -1 = right
  */
  int side = 1;
  bool mid = false;  

  /*
  * true = arcade.
  * false = tank.
  */
  bool move = false;
  bool stick = false;
  /*
  * 0 = Not run, use auton configuration buttons.
  * 1 = Auton is complete, just needs to clear screen.
  * 2 = Cleared the screen, no more to do.
  */
  int autonCompletitonSteps = 0;

  void autonomous(void) {
    GameObjects::inertialSensor.calibrate();
    waitUntil(!GameObjects::inertialSensor.isCalibrating());
    if (mid) {
      Auton::move(10, 1000);
      Auton::turnDeg(45 * side, 2000);
      Auton::move(5, 1000);
      Auton::intake(2000, Auton::globalSpeed);
      Auton::move(-5, 1000);
      Auton::turnDeg(135 * side, 4000);
      Auton::move(60.96, 8000);
      Auton::turnDeg(-45 * side, 2000);
      Auton::move(5, 1000);
      Auton::intake(2000, Auton::globalSpeed);
      Auton::move(-30, 2000);
    } else {
      GameObjects::intakeLeft.spin(directionType::rev, Auton::globalSpeed, rpm);
      GameObjects::intakeRight.spin(directionType::rev, Auton::globalSpeed, rpm);
      Auton::move(30.345, 3000);
      Auton::turnDeg(-45 * side, 5000);
      Auton::move(10, 2000);
      GameObjects::lift.spin(directionType::rev, Auton::globalSpeed, rpm);
      wait(2000, msec);
      Auton::move(-30, 2000);
      GameObjects::intakeLeft.stop(coast);
      GameObjects::intakeRight.stop(coast);
      GameObjects::lift.stop(coast);
    }
    autonCompletitonSteps++;
  }
  
  // Resets the controller screen, refresing the values.
  void resetControllerScreen() {
    GameObjects::Controller.Screen.clearScreen();
    
    // Cursor is where it prints on the screen.
    GameObjects::Controller.Screen.setCursor(1,1);
    GameObjects::Controller.Screen.print("Syntech:");

    // Print the state of the current movement on the controller.
    GameObjects::Controller.Screen.setCursor(2,1);
    if (move) GameObjects::Controller.Screen.print("Movement: arcade");
    else GameObjects::Controller.Screen.print("Movement: tank");
    
    // Prints teh state of if the movement is sticky on the controller.
    GameObjects::Controller.Screen.setCursor(3,1);
    if (stick) GameObjects::Controller.Screen.print("Sticky: true");
    else GameObjects::Controller.Screen.print("Sticky: false");
  }

  void usercontrol() {
    GameObjects::leftMotor.stop(coast);
    GameObjects::rightMotor.stop(coast);

    Brain.Screen.setFillColor("#0000000");     
    // Mid Label
    if (mid) Brain.Screen.printAt(40, 50, "Mid (l=true, r=false) : true"); 
    else Brain.Screen.printAt(40, 50, "Mid (l=true, r=false) : false");    
    // Mid Buttons
    UI::Button mid_true_button = UI::Button("#00ff00", Vector2f(40, 55), Vector2f::square(50), []{ mid = true; });
    UI::Button mid_false_button = UI::Button("#ff0000", Vector2f(90, 55), Vector2f::square(50), []{ mid = false; });
    
    // Side Label
    if (side == 1) Brain.Screen.printAt(40, 115, "Side : left");
    else Brain.Screen.printAt(40, 115, "Side : right");
    // Side Buttons
    UI::Button side_left_button = UI::Button("#000000", Vector2f(40, 120), Vector2f::square(50), []{ side = 1; });
    UI::Button side_right_button = UI::Button("#ffffff", Vector2f(90, 120), Vector2f::square(50), []{ side = -1; });
    
    resetControllerScreen();

    while (true) {
      Vector2f mousePos = Vector2f(Brain.Screen.xPosition(), Brain.Screen.yPosition());

      // If the autonmous is run, don't check button presses and clear screen.
      if (autonCompletitonSteps == 0) {
        // Runs functions of buttons through callbacks and clears the console if
        // one of them is true so that the labels refresh.
        if (UI::pollButtons(&mousePos)) {
          Console::reset();

          Brain.Screen.setFillColor("#0000000");
          // Mid Label
          if (mid) Brain.Screen.printAt(40, 50, "Mid (l=true, r=false) : true"); 
          else Brain.Screen.printAt(40, 50, "Mid (l=true, r=false) : false");
          // Mid Buttons
          mid_true_button.render();

          // Side Label
          if (side == 1) Brain.Screen.printAt(40, 115, "Side : left"); 
          else Brain.Screen.printAt(40, 115, "Side : right");

          // Side Buttons
          side_left_button.render();
          side_right_button.render();
        }
      } else if (autonCompletitonSteps == 1) {
        Console::reset();
        autonCompletitonSteps++;
      }

      // The difference between how fast the motors should  be going and the input.
      float diffL = (GameObjects::leftMotor.velocity(rpm) - GameObjects::Controller.Axis3.value()) * .12; // /.12 = / 60 / 360
      float diffR = (GameObjects::rightMotor.velocity(rpm) - GameObjects::Controller.Axis2.value()) * .12;
      
      if(!stick) {
        if(GameObjects::Controller.Axis3.value() == 0) diffL = 0;
        if(GameObjects::Controller.Axis2.value() == 0) diffR = 0;
      }
      
      if (!move) { // Tank
        GameObjects::leftMotor.spin(directionType::fwd, (GameObjects::Controller.Axis3.value() - diffL) * 2, velocityUnits::rpm);
        GameObjects::rightMotor.spin(directionType::fwd, (GameObjects::Controller.Axis2.value() - diffR) * 2, velocityUnits::rpm);
      } else { // Arcade
        GameObjects::leftMotor.spin(directionType::fwd, GameObjects::Controller.Axis3.value() + GameObjects::Controller.Axis4.value(), velocityUnits::rpm);
        GameObjects::rightMotor.spin(directionType::fwd, GameObjects::Controller.Axis3.value() - GameObjects::Controller.Axis4.value(), velocityUnits::rpm);
      }

      float intake = 999 * (GameObjects::Controller.ButtonL1.pressing() - GameObjects::Controller.ButtonL2.pressing());
      GameObjects::intakeLeft.spin(directionType::fwd, intake, velocityUnits::rpm);
      GameObjects::intakeRight.spin(directionType::fwd, intake, velocityUnits::rpm);
      GameObjects::lift.spin(directionType::fwd, intake, velocityUnits::rpm);
      // Holds the lift
      if (intake == 0) {
        GameObjects::lift.stop(hold);
      }

      GameObjects::Controller.ButtonX.pressed([]{ // Change stick
        stick = !stick; 
        resetControllerScreen();
      });
      GameObjects::Controller.ButtonY.pressed([]{ // Change move
        move = !move;
        resetControllerScreen();
      });

      wait(20, msec);
    }    
  }
}

int main() {
  vexcodeInit();
    
  Syntech::Console::reset(); // Inits console.

  Syntech::GameObjects::Competition.autonomous(Syntech::autonomous);
  Syntech::GameObjects::Competition.drivercontrol(Syntech::usercontrol);

  while (true) {
    wait(100, msec);
  }
}