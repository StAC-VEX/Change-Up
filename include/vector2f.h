#pragma once

/**
* This file/class represents a Vector with two dimensions.
*
* @author Bailey Moir
*/

namespace Syntech {
  class Vector2f {
  public:
    /**
    * The default constructor, this is bad programming practice.
    */
    Vector2f();

    /**
    * The prefered constructor.
    * @param a_x the value that x is set to.
    * @param a_y the value that y is set to.
    */
    Vector2f(float a_x, float a_y);
    
    /**
    * Acts like a constructor but isn't a literal constructor.
    * Creates a new vector with both values being the same.
    * @param value the value that x and y are set to.
    */
    static Vector2f square(float value);

    // Each dimension's value.
    float x, y;

    /**
    * An overload operator. Allows for situations like 'a + b' to occur (with both variables being Vectors).
    * @return A new Vector2f which has the values of the two Vectors put together.
    * @param &b The second Vector2f.
    */
    Vector2f operator+(const Vector2f *b);

    /**
    * An overload operator. Allows for situations like 'a * b' to occur (with both variables being Vectors).
    * @return A new Vector2f which has the values of the two Vectors timesed together.
    * @param &b The second Vector2f.
    */
    Vector2f operator*(const Vector2f *b);
  };
}