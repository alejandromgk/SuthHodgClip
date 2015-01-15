/*
 * Karol Alejandro Mancera Guapo
 * Primavera-Verano 2014
 * DICIS - UGTO
 * Gráficos por Computadora
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <cstdio>
#include <iostream>

class Point;

class Matrix
{
public:
  float** mData;
  int rows, cols;
  
  Matrix();
  Matrix(const int rows_, const int cols_);
  Matrix(const Matrix& m);
  ~Matrix();
  Matrix& operator=(const Matrix& m);
  Matrix& operator+(const Matrix& m);
  Matrix& operator-(const Matrix& m);
  Matrix operator*(const Matrix& m);
  float operator()(const int i, const int j);
  float* operator[](const int j);
  void printData();
  Matrix& identity();
};

#endif // MATRIX_H
