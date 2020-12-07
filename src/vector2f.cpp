#include "vector2f.h"

/**
* This file/class represents a Vector with two dimensions.
*
* @author Bailey Moir
*/

namespace Syntech {
  /**
  * The default constructor, this is bad programming practice.
  */
  Vector2f::Vector2f() {
    x = 0;
    y = 0;
  }

  /**
  * The prefered constructor.
  * @param a_x the value that x is set to.
  * @param a_y the value that y is set to.
  */
  Vector2f::Vector2f(float a_x, float a_y) {
    x = a_x;
    y = a_y;
  }
  
  /**
  * Acts like a constructor but isn't a literal constructor.
  * Creates a new vector with both values being the same.
  * @param value the value that x and y are set to.
  */  
  Vector2f Vector2f::square(float value) {
    return Vector2f(value, value);
  }
  
  /**
  * An overload operator. Allows for situations like 'a + b' to occur (with both variables being Vectors).
  * @return A new Vector2f which has the values of the two Vectors put together.
  * @param &b The second Vector2f.
  */
  Vector2f Vector2f::operator+(const Vector2f *b) {
    return Vector2f(this->x + (*b).x, this->y + (*b).y);
  }

  /**
  * An overload operator. Allows for situations like 'a * b' to occur (with both variables being Vectors).
  * @return A new Vector2f which has the values of the two Vectors timesed together.
  * @param &b The second Vector2f.
  */
  Vector2f Vector2f::operator*(const Vector2f *b) {
    return Vector2f(this->x * (*b).x, this->y * (*b).y);
  }
}