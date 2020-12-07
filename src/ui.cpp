#include "ui.h"

/**
* This file is for the class that manages UI
*
* @author Bailey Moir
*/

namespace Syntech {
  /** 
  * Checks to see if a point is in a rectangle.
  * @param startPos the position of the top left corner of the rectangle.
  * @param size the width and height of the rectangle.
  * @param point the point being tested to see if it is inside the rectangle.
  * @return if it is in the rectangle or not.
  */
  bool UI::inRectangle(Vector2f startPos, Vector2f size, Vector2f point) {
    return (point.x < startPos.x + size.x && point.x > startPos.x && point.y < startPos.y + size.y && point.y > startPos.y ? true : false);
    // Closer than right, further than left, Closer than top, further than bottom
  }

  /**
  * Draws/renders a rectangle to the screen.
  * @param color the color of the rectangle.
  * @param startPos the position of the top left corner of the rectangle.
  * @param size the width and height of the rectangle.
  */
  void UI::drawRectangle(std::string color, Vector2f startPos, Vector2f size) { 
      Brain.Screen.setFillColor( color.c_str() );
      Brain.Screen.drawRectangle( startPos.x, startPos.y, size.x, size.y );
  }

  bool UI::pollButtons(Vector2f *point) {
    for (Button *btn : m_buttons) {
      if ((*btn).poll(point)) return true;
    }
    return false;
  }

  /**
  * The contructor.
  * @param color the color of the box.
  * @param startPos the position of the top left corner of the button.
  * @param size the width and height of the button.
  * @param callback the function that runs when the button is pressed.
  */
  UI::Button::Button(std::string a_color, Vector2f a_startPos, Vector2f a_size, void (*callback)()) {
    color = a_color;
    startPos = a_startPos;
    size = a_size;

    m_callback = callback;
    *m_buttons[len(m_buttons)] = this;

    // Render button
    Brain.Screen.setFillColor( color.c_str() );
    Brain.Screen.drawRectangle( startPos.x, startPos.y, size.x, size.y );
  }

  /**
  * Returns if the button is being pressed, and if it is it runs the callback function.
  * @param point the point at which the screen is being pressed.
  * @return if the point is inside the button. 
  */
  bool UI::Button::poll(Vector2f *point) {
    // Closer than right, further than left, Closer than top, further than bottom (i.e. IN BOX)
    if ((*point).x < startPos.x + size.x && (*point).x > startPos.x && (*point).y < startPos.y + size.y && (*point).y > startPos.y && Brain.Screen.pressing()) {
      // Run callback
      ((void(*)())m_callback)();
      return true;
    } else {
      return false;
    }
  }

  /**
  * Renders the button.
  */
  void UI::Button::render() {    
    Brain.Screen.setFillColor( color.c_str() );
    Brain.Screen.drawRectangle( startPos.x, startPos.y, size.x, size.y );
  }

  UI::Button UI::Button::operator=(Button *b) {
    return *b;
  }
}