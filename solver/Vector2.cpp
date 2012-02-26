#include "Vector2.h"

// Default constructor initializes the array elements to zero
Vector2::Vector2() : x(0.0f), y(0.0f) {}

Vector2::Vector2(float nx, float ny) : x(nx), y(ny) {}

Vector2& Vector2::zero()
{
  (*this).x = (*this).y = 0.0f;
  return *this;
}

Vector2& Vector2::zeroX()
{
  (*this).x = 0.0f;
  return *this;
}

Vector2& Vector2::zeroY()
{
  (*this).y = 0.0f;
  return *this;
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
  else
    return *this;
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
