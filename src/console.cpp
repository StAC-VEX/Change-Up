#include "console.h"

/**
* The file and class that represents the console on the V5 Brain.
*
* @auton Bailey Moir
*/

namespace Syntech {
  // Sets the color.
  std::string Console::foreground = "#841B13";
  // Sets the current line.
  int Console::m_Line = 0;

  /**
  * Logs a string to the console.
  * @param text The text to print.
  */
  void Console::log(std::string text) {
    std::stringstream textS;
    textS << "> " << text;
    
    // Prints text
    Brain.Screen.setPenColor("#ffffff");
    Brain.Screen.setFillColor(foreground.c_str());
    Brain.Screen.printAt(40, 60 + m_Line*20, textS.str().c_str());
    m_Line++;
  }
  
  /**
  * Sets the active line.
  * @param line the line you want to be on.
  */
  void Console::setLine(int line) {
    m_Line = line;
  }

  /**
  * Clears the screen whilst rendering the default console.
  */
  void Console::reset() {
    Brain.Screen.clearScreen();
    // Draws both rectangles.
    UI::drawRectangle("#000000", Vector2f(0, 0), Vector2f(480, 272));
    UI::drawRectangle(foreground.c_str(), Vector2f::square(30), Vector2f(420, 180));
  }
}