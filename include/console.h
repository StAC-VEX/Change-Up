#pragma once
#include "vex.h"
#include "ui.h"
#include "sstream"

/**
* The file and class that represents the console on the V5 Brain.
*
* @auton Bailey Moir
*/

namespace Syntech {
  class Console {
  public:
    /**
    * Logs a string to the console.
    * @param text The text to print.
    */
    static void log(std::string text);

    /**
    * Sets the active line.
    * @param line the line you want to be on.
    */
    static void setLine(int line);
    
    /**
    * Clears the screen whilst rendering the default console.
    */
    static void reset();

    // The color of the foreground.
    static std::string foreground;
  private:
    // The current line
    static int m_Line;
  };
}