/*
 * Karol Alejandro Mancera Guapo
 * Primavera-Verano 2014
 * DICIS - UGTO
 * Gráficos por Computadora
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <cstdio>
#include <vector>
#include <iostream>

class Point
{
public:
  float x, y, z;
  int d;
  
  Point();
  Point(const float x, const float y, const float z);
  Point(const float x, const float y);
  ~Point();
  operator float*();
//   float &operator[]( int n );
//   float operator[]( int n ) const;
  Point operator+( const Point &v ) const;
  Point operator-() const;
  Point operator-( const Point &v ) const;
  Point operator*( float s ) const;
  Point operator*( const Point &v ) const;
  Point operator/( float s ) const;  
  void toString();
};

class PointArray
{
public:
  std::vector<Point> pt;
  int yMin();
  int yMax();
  static bool compY(Point p1, Point p2);
};

#endif // CVECTOR_H
