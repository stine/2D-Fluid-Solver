#ifndef __VECTOR_2_TEST__
#define __VECTOR_2_TEST__

#include <gtest/gtest.h>
#include <cmath>
#include "Vector2.h"

TEST(Vector2Test, DefaultConstructor)
{
  const Vector2 defaultVec;
  EXPECT_EQ(defaultVec.x, 0.0f);
  EXPECT_EQ(defaultVec.y, 0.0f);
}

TEST(Vector2Test, ValueConstructor)
{
  const Vector2 defaultVec(5.0f, -5.0f);
  EXPECT_EQ(5.0f, defaultVec.x);
  EXPECT_EQ(-5.0f, defaultVec.y);
}

TEST(Vector2Test, ElementAssignment) 
{
  Vector2 vec;
  vec.x = 5.0f;
  vec.y = 10.0f;
  EXPECT_EQ(5.0f, vec.x);
  EXPECT_EQ(10.0f, vec.y);
}

TEST(Vector2Test, ExplicitInit)
{
  Vector2 vec(2.0f, 10.0f);
  EXPECT_EQ(2.0f, vec.x);
  EXPECT_EQ(10.0f, vec.y);
}

TEST(Vector2Test, Negate)
{
  Vector2 vec(2.0f, 10.0f);
  Vector2 result = vec.negate();
  EXPECT_EQ(-2.0f, result.x);
  EXPECT_EQ(-10.0f, result.y);
}

TEST(Vector2Test, NegateX)
{
  Vector2 vec(2.0f, 10.0f);
  Vector2 result = vec.negateX();
  EXPECT_EQ(-2.0f, result.x);
  EXPECT_EQ(10.0f, result.y);
}

TEST(Vector2Test, NegateY)
{
  Vector2 vec(2.0f, 10.0f);
  Vector2 result = vec.negateY();
  EXPECT_EQ(2.0f, result.x);
  EXPECT_EQ(-10.0f, result.y);
}

TEST(Vector2Test, Zero)
{
  Vector2 vec(2.0f, 10.0f);
  vec.zero();
  EXPECT_EQ(0.0f, vec.x);
  EXPECT_EQ(0.0f, vec.y);
}


TEST(Vector2Test, ZeroX)
{
  Vector2 vec(2.0f, 10.0f);
  vec.zeroX();
  EXPECT_EQ(0.0f, vec.x);
  EXPECT_EQ(10.0f, vec.y);
}

TEST(Vector2Test, ZeroY)
{
  Vector2 vec(2.0f, 10.0f);
  vec.zeroY();
  EXPECT_EQ(2.0f, vec.x);
  EXPECT_EQ(0.0f, vec.y);
}

TEST(Vector2Test, Magnitude) 
{
  Vector2 vec;
  vec.x = 0.0f;
  vec.y = 0.0f;
  EXPECT_EQ(0.0f, vec.magnitude());
  vec.x = 0.0f;
  vec.y = 10.0f;
  EXPECT_EQ(10.0f, vec.magnitude());
  vec.x = 0.0f;
  vec.y = -10.0f;
  EXPECT_EQ(10.0f, vec.magnitude());
  vec.x = -3.0f;
  vec.y = -4.0f;
  EXPECT_EQ(5.0f, vec.magnitude());
}

TEST(Vector2Test, Unit) 
{
  Vector2 vec;
  Vector2 unitVec;
  vec.x = 0.0f;
  vec.y = 0.0f;
  unitVec = vec.unit();
  EXPECT_EQ(0.0f, unitVec.x);
  EXPECT_EQ(0.0f, unitVec.y);
  vec.x = -1.0f;
  vec.y =  0.0f;
  unitVec = vec.unit();
  EXPECT_EQ(-1.0f, unitVec.x);
  EXPECT_EQ(0.0f, unitVec.y);
  vec.x = 0.0f;
  vec.y = 5.0f;
  unitVec = vec.unit();
  EXPECT_EQ(0.0f, unitVec.x);
  EXPECT_EQ(1.0f, unitVec.y);
  vec.x = 3.0f;
  vec.y = 4.0f;
  unitVec = vec.unit();
  EXPECT_EQ(0.6f, unitVec.x);
  EXPECT_EQ(0.8f, unitVec.y);
}

TEST(Vector2Test, Normalize) 
{
  Vector2 vec;
  vec.x = 0.0f;
  vec.y = 0.0f;
  vec.normalize();
  EXPECT_EQ(0.0f, vec.x);
  EXPECT_EQ(0.0f, vec.y);
  vec.x = -1.0f;
  vec.y =  0.0f;
  vec.normalize();
  EXPECT_EQ(-1.0f, vec.x);
  EXPECT_EQ(0.0f, vec.y);
  vec.x = 0.0f;
  vec.y = 5.0f;
  vec.normalize();
  EXPECT_EQ(0.0f, vec.x);
  EXPECT_EQ(1.0f, vec.y);
  vec.x = 3.0f;
  vec.y = 4.0f;
  vec.normalize();
  EXPECT_EQ(0.6f, vec.x);
  EXPECT_EQ(0.8f, vec.y);
}

TEST(Vector2Test, Dot) 
{
  Vector2 vecA, vecB;
  float result;
  vecA.x = 0.0f;
  vecA.y = 1.0f;
  vecB.x = 1.0f;
  vecB.y = 2.0f;
  result = vecA.dot(vecB);
  EXPECT_EQ(2.0f, result);
}

TEST(Vector2Test, Addition) 
{
  Vector2 vecA, vecB, vecC, vecD;
  vecA.x = 0.0f;
  vecA.y = 1.0f;
  vecB.x = 1.0f;
  vecB.y = 2.0f;
  vecC = vecA + vecB;
  EXPECT_EQ(1.0f, vecC.x);
  EXPECT_EQ(3.0f, vecC.y);  
  vecD += vecA;
  EXPECT_EQ(0.0f, vecD.x);
  EXPECT_EQ(1.0f, vecD.y);  
}

TEST(Vector2Test, Subtraction) 
{
  Vector2 vecA, vecB, vecC, vecD;
  vecA.x = 0.0f;
  vecA.y = 1.0f;
  vecB.x = 1.0f;
  vecB.y = 2.0f;
  vecC = vecB - vecA;
  EXPECT_EQ(1.0f, vecC.x);
  EXPECT_EQ(1.0f, vecC.y);  
  vecD -= vecA;
  EXPECT_EQ(0.0f, vecD.x);
  EXPECT_EQ(-1.0f, vecD.y);  
}

TEST(Vector2Test, Multiplication) 
{
  Vector2 vec, result;
  vec.x =  10.0f;
  vec.y = -10.0f;
  result = vec * 8;
  EXPECT_EQ(80.0f, result.x);
  EXPECT_EQ(-80.0f, result.y);  
  vec.x = 1340.0f;
  vec.y = 5000.0f;
  vec *= 0;
  EXPECT_EQ(0.0f, vec.x);
  EXPECT_EQ(0.0f, vec.y);  
}

TEST(Vector2Test, Division) 
{
  Vector2 vec, result;
  vec.x =  10.0f;
  vec.y = -10.0f;
  result = vec / 2;
  EXPECT_EQ(5.0f, result.x);
  EXPECT_EQ(-5.0f, result.y);  
}

#endif // __VECTOR_2_TEST__
