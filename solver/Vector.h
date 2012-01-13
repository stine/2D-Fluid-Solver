#ifndef __VECTOR_H__
#define __VECTOR_H__

#include<math.h>

// This is a class to perform standard mathematical operations typical to vector classes.
template <unsigned N, class T = float>
class Vector {
  T _val[N];

public:
  // Default Constructor
  Vector();
  //Vector<N,T>(T * first element of array);

  // Element access 
  T& operator()(const unsigned &index);
  T operator()(const unsigned &index) const;

  // Vector math operators
  T magnitude() const;
  Vector<N, T> unit() const;
  Vector<N, T>& normalize();
  T dot(const Vector<N, T> &rhs) const;
  Vector<N, T> operator+(const Vector<N, T> &rhs) const;
  Vector<N, T> operator-(const Vector<N, T> &rhs) const;
  Vector<N, T> operator*(const T &rhs) const;
  Vector<N, T> operator/(const T &rhs) const;
  Vector<N, T>& operator+=(const Vector<N, T> &rhs);
  Vector<N, T>& operator-=(const Vector<N, T> &rhs);
  Vector<N, T>& operator*=(const T &rhs);
  Vector<N, T>& operator/=(const T &rhs);
  // TODO cross product; specialize template for 3D.
  
  // Comparison operators
  bool operator==(const Vector<N, T> &rhs) const;
  bool operator!=(const Vector<N, T> &rhs) const;
};

// Default constructor initializes the array elements to zero
template<unsigned N, class T>
Vector<N, T>::Vector()
{
  for (unsigned i = 0; i < N; ++i)
    _val[i] = 0;
}

// Element access operator overloading
template<unsigned N, class T>
T& Vector<N,T>::operator()(const unsigned &index)
{
  return _val[index];
}

template<unsigned N, class T>
T Vector<N,T>::operator()(const unsigned &index) const
{  
  return _val[index];
}

template<unsigned N, class T>
T Vector<N, T>::magnitude() const
{
  T result = 0;
  for (unsigned i = 0; i < N; ++i){
    result += _val[i] * _val[i];
  }
  return sqrt(result);
}

template<unsigned N, class T>
Vector<N, T>& Vector<N, T>::normalize()
{
  *this = unit();
  return *this;
}

template<unsigned N, class T>
Vector<N, T> Vector<N, T>::unit() const
{
  Vector<N, T> result;
  T mag = (*this).magnitude();
  if (mag != 0) {
    result = *this / mag;
  }
  return result;
}

template<unsigned N, class T>
T Vector<N, T>::dot(const Vector<N, T> &rhs) const
{
  T result = 0;
  for (unsigned i = 0; i < N; ++i) {
    result += _val[i] * rhs(i);
  }
  return result;
}

// Binary operator overloading
template<unsigned N, class T>
Vector<N, T> Vector<N, T>::operator+(const Vector<N, T> &rhs) const
{
  Vector<N, T> result;
  for (unsigned i = 0; i < N; ++i) {
    result(i) = _val[i] + rhs(i);
  }
  return result;
}

template<unsigned N, class T>
Vector<N, T> Vector<N, T>::operator-(const Vector<N, T> &rhs) const
{
  Vector<N, T> result;
  for (unsigned i = 0; i < N; ++i) {
    result(i) = _val[i] - rhs(i);
  }
  return result;
}

template<unsigned N, class T>
Vector<N, T> Vector<N, T>::operator*(const T &rhs) const
{
  Vector<N, T> result;
  for (unsigned i = 0; i < N; ++i) {
    result(i) = _val[i] * rhs;
  }
  return result;
}

template<unsigned N, class T>
Vector<N, T> Vector<N, T>::operator/(const T &rhs) const
{
  Vector<N, T> result;
  for (unsigned i = 0; i < N; ++i) {
    result(i) = _val[i] / rhs;
  }
  return result;
}

template<unsigned N, class T>
Vector<N, T>& Vector<N, T>::operator+=(const Vector<N, T> &rhs)
{
  *this = *this + rhs; 
  return *this;
}

template<unsigned N, class T>
Vector<N, T>& Vector<N, T>::operator-=(const Vector<N, T> &rhs)
{
  *this = *this - rhs;
  return *this;
}

template<unsigned N, class T>
Vector<N, T>& Vector<N, T>::operator*=(const T &rhs)
{
  *this = *this * rhs;
  return *this;
}

template<unsigned N, class T>
Vector<N, T>& Vector<N, T>::operator/=(const T &rhs)
{
  *this = *this / rhs;
  return *this;
}

// Comparison operator overloading
template<unsigned N, class T>
bool Vector<N, T>::operator==(const Vector<N, T> &rhs) const
{
  bool eql = true;
  for (unsigned i = 0; i < N; ++i) {
    if (_val[i] != rhs(i)) {
      eql = false;
      break;
    }
  }
  return eql;
}

template<unsigned N, class T>
bool Vector<N, T>::operator!=(const Vector<N, T> &rhs) const
{
  return !(*this == rhs);
}


#endif //__VECTOR_H__

