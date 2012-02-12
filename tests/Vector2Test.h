#ifndef __VECTOR_2_TEST__
#define __VECTOR_2_TEST__

#include <gtest/gtest.h>
#include <cmath>
#include "Vector.h"

TEST(Vector2Test, DefaultConstructor)
{
  const Vector<2,float> defaultVec;
  EXPECT_EQ(defaultVec(0), 0.0f);
  EXPECT_EQ(defaultVec(1), 0.0f);
}

TEST(Vector2Test, ValueConstructor)
{
  const Vector<2,float> defaultVec(5.0f, -5.0f);
  EXPECT_EQ(defaultVec(0), 5.0f);
  EXPECT_EQ(defaultVec(1), -5.0f);
}

TEST(Vector2Test, ElementAssignment) 
{
  Vector<2,float> vec;
  vec(0) = 5.0f;
  vec(1) = 10.0f;
  EXPECT_EQ(vec(0), 5.0f);
  EXPECT_EQ(vec(1), 10.0f);
}

TEST(Vector2Test, Magnitude) 
{
  Vector<2,float> vec;
  vec(0) = 0.0f;
  vec(1) = 0.0f;
  EXPECT_EQ(vec.magnitude(), 0.0f);
  vec(0) = 0.0f;
  vec(1) = 10.0f;
  EXPECT_EQ(vec.magnitude(), 10.0f);
  vec(0) = 0.0f;
  vec(1) = -10.0f;
  EXPECT_EQ(vec.magnitude(), 10.0f);
  vec(0) = -3.0f;
  vec(1) = -4.0f;
  EXPECT_EQ(vec.magnitude(), 5.0f);
}

TEST(Vector2Test, Unit) 
{
  Vector<2,float> vec;
  Vector<2,float> unitVec;
  vec(0) = 0.0f;
  vec(1) = 0.0f;
  unitVec = vec.unit();
  EXPECT_EQ(unitVec(0), 0.0f);
  EXPECT_EQ(unitVec(1), 0.0f);
  vec(0) = -1.0f;
  vec(1) =  0.0f;
  unitVec = vec.unit();
  EXPECT_EQ(unitVec(0), -1.0f);
  EXPECT_EQ(unitVec(1),  0.0f);
  vec(0) = 0.0f;
  vec(1) = 5.0f;
  unitVec = vec.unit();
  EXPECT_EQ(unitVec(0), 0.0f);
  EXPECT_EQ(unitVec(1), 1.0f);
  vec(0) = 3.0f;
  vec(1) = 4.0f;
  unitVec = vec.unit();
  EXPECT_EQ(unitVec(0), 0.6f);
  EXPECT_EQ(unitVec(1), 0.8f);
}

TEST(Vector2Test, Normalize) 
{
  Vector<2,float> vec;
  vec(0) = 0.0f;
  vec(1) = 0.0f;
  vec.normalize();
  EXPECT_EQ(vec(0), 0.0f);
  EXPECT_EQ(vec(1), 0.0f);
  vec(0) = -1.0f;
  vec(1) =  0.0f;
  vec.normalize();
  EXPECT_EQ(vec(0), -1.0f);
  EXPECT_EQ(vec(1),  0.0f);
  vec(0) = 0.0f;
  vec(1) = 5.0f;
  vec.normalize();
  EXPECT_EQ(vec(0), 0.0f);
  EXPECT_EQ(vec(1), 1.0f);
  vec(0) = 3.0f;
  vec(1) = 4.0f;
  vec.normalize();
  EXPECT_EQ(vec(0), 0.6f);
  EXPECT_EQ(vec(1), 0.8f);
}

TEST(Vector2Test, Dot) 
{
  Vector<2,float> vecA, vecB;
  float result;
  vecA(0) = 0.0f;
  vecA(1) = 1.0f;
  vecB(0) = 1.0f;
  vecB(1) = 2.0f;
  result = vecA.dot(vecB);
  EXPECT_EQ(result, 2.0f);
}

TEST(Vector2Test, Addition) 
{
  Vector<2,float> vecA, vecB, vecC, vecD;
  vecA(0) = 0.0f;
  vecA(1) = 1.0f;
  vecB(0) = 1.0f;
  vecB(1) = 2.0f;
  vecC = vecA + vecB;
  EXPECT_EQ(vecC(0), 1.0f);
  EXPECT_EQ(vecC(1), 3.0f);  
  vecD += vecA;
  EXPECT_EQ(vecD(0), 0.0f);
  EXPECT_EQ(vecD(1), 1.0f);  
}

TEST(Vector2Test, Subtraction) 
{
  Vector<2,float> vecA, vecB, vecC, vecD;
  vecA(0) = 0.0f;
  vecA(1) = 1.0f;
  vecB(0) = 1.0f;
  vecB(1) = 2.0f;
  vecC = vecB - vecA;
  EXPECT_EQ(vecC(0), 1.0f);
  EXPECT_EQ(vecC(1), 1.0f);  
  vecD -= vecA;
  EXPECT_EQ(vecD(0),  0.0f);
  EXPECT_EQ(vecD(1), -1.0f);  
}

TEST(Vector2Test, Multiplication) 
{
  Vector<2,float> vec, result;
  vec(0) =  10.0f;
  vec(1) = -10.0f;
  result = vec * 8;
  EXPECT_EQ(result(0),  80.0f);
  EXPECT_EQ(result(1), -80.0f);  
  vec(0) = 1340.0f;
  vec(1) = 5000.0f;
  vec *= 0;
  EXPECT_EQ(vec(0), 0.0f);
  EXPECT_EQ(vec(1), 0.0f);  
}

TEST(Vector2Test, Division) 
{
  Vector<2,float> vec, result;
  vec(0) =  10.0f;
  vec(1) = -10.0f;
  result = vec / 2;
  EXPECT_EQ(result(0),  5.0f);
  EXPECT_EQ(result(1), -5.0f);  
}

#endif // __VECTOR_2_TEST__
