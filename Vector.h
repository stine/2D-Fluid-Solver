#ifndef __VECTOR_H__
#define __VECTOR_H__

template <unsigned N, class T = float>
class Vector {
  T _val[N];

public:
  // Default Constructor
  Vector();

  // Element access 
  T& operator()(const unsigned &index);
  T operator()(const unsigned &index) const;

  // Vector math operators
  T magnitude() const;
  Vector<N, T> unit() const;
  void normalize();
  T dot(const Vector<N, T> &rhs) const;
  Vector<N, T> operator*(const T &rhs) const;
  Vector<N, T> operator/(const T &rhs) const;
  Vector<N, T> operator+(const Vector<N, T> &rhs) const;
  Vector<N, T> operator-(const Vector<N, T> &rhs) const;
  Vector<N, T>& operator*=(const T &rhs);
  Vector<N, T>& operator/=(const T &rhs);
  Vector<N, T>& operator+=(const Vector<N, T> &rhs);
  Vector<N, T>& operator-=(const Vector<N, T> &rhs);
  // TODO cross product; specialize template for 3D.
  
  // Comparison operators
  bool operator==(const Vector<N, T> &rhs) const;
  bool operator!=(const Vector<N, T> &rhs) const;
};


template<unsigned N, class T>
Vector<N, T>::Vector()
{
  for (unsigned i = 0; i < N; ++i)
    _val[i] = 0;
}


// TODO define methods as needed.


#endif //__VECTOR_H__
