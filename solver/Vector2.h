#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include<math.h>

// This is a class to perform standard mathematical operations typical to vector classes.
class Vector2 {
public:
  float x, y;
  // Constructors
  Vector2();
  Vector2(float nx, float ny);

  // Vector math operators
  Vector2& zero();
  Vector2& zeroX();
  Vector2& zeroY();
  Vector2 negate() const;
  Vector2 negateX() const;
  Vector2 negateY() const;
  float magnitude() const;
  Vector2 unit() const;
  Vector2& normalize();
  float dot(const Vector2 &rhs) const;
  Vector2 operator+(const Vector2 &rhs) const;
  Vector2 operator-(const Vector2 &rhs) const;
  Vector2 operator*(const float &rhs) const;
  Vector2 operator/(const float &rhs) const;
  Vector2& operator+=(const Vector2 &rhs);
  Vector2& operator-=(const Vector2 &rhs);
  Vector2& operator*=(const float &rhs);
  Vector2& operator/=(const float &rhs);

  // Comparison operators
  bool operator==(const Vector2 &rhs) const;
  bool operator!=(const Vector2 &rhs) const;
};

#endif // __VECTOR2_H__
