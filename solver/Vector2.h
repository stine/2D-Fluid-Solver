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

// Default constructor initializes the array elements to zero
Vector2::Vector2() : x(0.0f), y(0.0f) {}

Vector2::Vector2(float nx, float ny) : x(nx), y(ny) {}

Vector2& Vector2::zero()
{
  x = y = 0.0f;
}

Vector2& Vector2::zeroX()
{
  x = 0.0f; 
}

Vector2& Vector2::zeroY()
{
  y = 0.0f;
}

Vector2 Vector2::negate() const
{
  return Vector2(x, y) * -1.0f;
}

Vector2 Vector2::negateX() const
{
  return Vector2(x * -1.0f, y);
}

Vector2 Vector2::negateY() const
{
  return Vector2(x, y * -1.0f);
}

float Vector2::magnitude() const
{
  return sqrt(x * x + y * y);
}

Vector2& Vector2::normalize()
{
  *this = unit();
  return *this;
}

Vector2 Vector2::unit() const
{
  float mag = (*this).magnitude();
  if (mag != 0) 
    return *this / mag;
}

float Vector2::dot(const Vector2 &rhs) const
{
  return x * rhs.x + y * rhs.y;
}

// Binary operator overloading
Vector2 Vector2::operator+(const Vector2 &rhs) const
{
  return Vector2(x + rhs.x, y + rhs.y);
}

Vector2 Vector2::operator-(const Vector2 &rhs) const
{
  return Vector2(x - rhs.x, y - rhs.y);
}

Vector2 Vector2::operator*(const float &rhs) const
{
  return Vector2(x * rhs, y * rhs);
}

Vector2 Vector2::operator/(const float &rhs) const
{
  return Vector2(x / rhs, y / rhs);
}

Vector2& Vector2::operator+=(const Vector2 &rhs)
{
  *this = *this + rhs; 
  return *this;
}

Vector2& Vector2::operator-=(const Vector2 &rhs)
{
  *this = *this - rhs;
  return *this;
}

Vector2& Vector2::operator*=(const float &rhs)
{
  *this = *this * rhs;
  return *this;
}

Vector2& Vector2::operator/=(const float &rhs)
{
  *this = *this / rhs;
  return *this;
}

// Comparison operator overloading
bool Vector2::operator==(const Vector2 &rhs) const
{
  bool eql = true;
  if(x != rhs.x || y != rhs.y)
    eql = false;
  return eql;
}

bool Vector2::operator!=(const Vector2 &rhs) const
{
  return !(*this == rhs);
}


#endif //__VECTOR2_H__

