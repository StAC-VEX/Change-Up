#pragma once
#include "vex.h"
#include "vector2f.h"

/**
* This file is for the class that manages UI
*
* @author Bailey Moir
*/

namespace Syntech {
  class UI {
  public:
    class Button {
    private:
      // The function that is run when the button is pressed.
      void (*m_callback)();
    public:
      /**
      * The contructor.
      * @param color the color of the box.
      * @param startPos the position of the top left corner of the button.
      * @param size the width and height of the button.
      * @param callback the function that runs when the button is pressed.
      */
      Button(std::string color, Vector2f startPos, Vector2f size, void(*callback)());

      /**
      * Returns if the button is being pressed, and if it is it runs the callback function.
      * @param point the point at which the screen is being pressed.
      * @return if the point is inside the button. 
      */
      bool poll(Vector2f *point);
      /**
      * Renders the button.
      */
      void render();

      // The color of the button
      std::string color;

      // The position of the top left corner of the button, and the width and height of the button.
      Vector2f startPos, size;

      Button operator=(Button *b);
    };

    static bool pollButtons(Vector2f *point);

    /** 
    * Checks to see if a point is in a rectangle.
    * @param startPos the position of the top left corner of the rectangle.
    * @param size the width and height of the rectangle.
    * @param point the point being tested to see if it is inside the rectangle.
    * @return if it is in the rectangle or not.
    */
    static bool inRectangle(Vector2f startPos, Vector2f size, Vector2f point);

    /**
    * Draws/renders a rectangle to the screen.
    * @param color the color of the rectangle.
    * @param startPos the position of the top left corner of the rectangle.
    * @param size the width and height of the rectangle.
    */
    static void drawRectangle(std::string color, Vector2f startPos, Vector2f size);
  private:
    static Button* m_buttons[0];
  };
}