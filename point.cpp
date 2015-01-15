/*
 * Karol Alejandro Mancera Guapo
 * Primavera-Verano 2014
 * DICIS - UGTO
 * Gráficos por Computadora
 */

#include "point.h"

Point::Point() : x(0),  y(0),  z(0) { }

Point::Point ( const float x, const float y, const float z) : x(x), y(y), z(z), d(3) { }

Point::Point ( const float x, const float y) : x(x), y(y), z(0), d(2) { }

Point::~Point() { }

Point Point::operator+(const Point& v) const
{
  return Point(x+v.x, y+v.y, z+v.z);
}

Point Point::operator-() const
{
  return Point(-x, -y, -z);
}

 Point Point::operator-(const Point& v) const
{
 return Point(x-v.x, y-v.y, z-v.z);
}

Point Point::operator*(float s) const
{
  return Point(x*s, y*s, z*s);
}

Point Point::operator*(const Point& v) const
{
  return Point(x*v.x, y*v.y, z*v.z);
}

Point Point::operator/(float s) const
{
  if (s != 0) {
    return Point(x/s, y/s, z/s);
  } else {
        std::cerr << "\nERROR: No se puede dividir por cero.\n" << std::endl;
  }
}

void Point::toString()
{
  std::printf("(%.2f, %.2f, %.2f)\n", x, y, z);
}

int PointArray::yMin()
{
  std::vector<Point> tmp = pt;
  std::sort(tmp.begin(), tmp.end(), this->compY);
  
  return (int)tmp[0].y;
}

int PointArray::yMax()
{
  std::vector<Point> tmp = pt;
  std::sort(tmp.begin(), tmp.end(), this->compY);
  
  return (int)tmp[tmp.size()-1].y;
}

bool PointArray::compY(Point p1, Point p2)
{
  return (p1.y < p2.y);
}
