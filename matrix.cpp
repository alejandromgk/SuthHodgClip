/*
 * Karol Alejandro Mancera Guapo
 * Primavera-Verano 2014
 * DICIS - UGTO
 * Gráficos por Computadora
 */

#include "matrix.h"

Matrix::Matrix() : rows(0), cols(0)
{
  mData = nullptr;
}

Matrix::Matrix(const int rows_, const int cols_) : rows(rows_), cols(cols_)
{
  mData = nullptr;
  if (rows > 0 && cols > 0) {
    mData = new float* [rows];
    for ( int i = 0; i < rows; i++ ) {
      mData[i] = new float[cols];
      for ( int j = 0; j < cols; j++ ) {
	this->mData[i][j] = 0;
      }
    }
  } else {
    std::cerr << "\nERROR: Los índices deben ser mayores a cero.\n" << std::endl;
  }
}

Matrix::Matrix(const Matrix& m)
{
  rows = m.rows;
  cols = m.cols;
  mData = new float*[rows];
  for ( int i = 0; i < rows; i++ ) {
    mData[i] = new float [cols];
    for ( int j = 0; j < cols; j++ ) {
      mData[i][j] = m.mData[i][j];
    }
  }
}

Matrix::~Matrix()
{
  for ( int i = 0; i < rows; i++) {
    delete mData[i];
  }
  delete mData;
  mData = nullptr;
}

Matrix& Matrix::operator=(const Matrix& m)
{
  rows = m.rows;
  cols = m.cols;
  mData = new float* [rows];
  for ( int i = 0; i < rows; i++ ) {
    mData[i] = new float [cols];
    for ( int j = 0; j < cols; j++ ) {
      mData[i][j] = m.mData[i][j];
    }
  }
}

Matrix& Matrix::operator+(const Matrix& m)
{
  if (rows == m.rows && cols == m.cols){
//     Matrix tmp(m.rows, m.cols);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
	mData[i][j] = mData[i][j] + m.mData[i][j];
      }
    }
    return *this;
  } else {
    std::cerr << "\nERROR: El tamaño de ambas matrices debe ser mxm.\n" << std::endl;
  }
}

Matrix& Matrix::operator-(const Matrix& m)
{
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      mData[i][j] = mData[i][j] - m.mData[i][j];
    }
  }
  return *this;
}

Matrix Matrix::operator*(const Matrix& m)
{
//   std::printf("local [%d, %d] argumento [%d, %d]", rows, cols, m.rows, m.cols);
  if (rows == m.rows) {
    Matrix tmp(rows,  m.cols);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < m.cols; j++) {
	int tmpSum = 0;
	for (int k = 0; k < rows; k++) {
	  tmpSum += mData[i][k] * m.mData[k][j];
	}
	tmp.mData[i][j] = tmpSum;
      }
    }
    return tmp;
  } else {
    std::cerr << "\nERROR: El tamaño de ambas matrices debe ser mxm o,  [mxn * nxm].\n" << std::endl;
  }
}

float Matrix::operator()(const int i, const int j)
{
  return mData[i][j];
}

float* Matrix::operator[](const int j)
{
  if (j<rows) {
    return &mData[j][0];
  } else {
    //std::perror("\nERROR: Los índices no coinciden con el tamaño de la matriz.\n\n");
    std::cerr << "\nERROR operator[]: Los índices no coinciden con el tamaño de la matriz.\n" << std::endl;
  }
}

void Matrix::printData()
{
  std::printf("\nMatriz de tamaño [%dx%d]:\n\n",  rows,  cols);
  for (int i = 0; i < rows; i++) {
    std::printf("\t");
    for (int j = 0; j < cols; j++) {
      std::printf("%.1f   ", this->mData[i][j]);
    }
      std::printf("\n");
  }
}

Matrix& Matrix::identity()
{
  if (rows > 1 && cols > 1 && rows == cols) {
    for (int i = 0; i < rows; i++) {
    mData[i][i] = 1;
    }
    return *this;
  } else {
    std::cerr << "\nERROR: La matriz debe ser cuadrada.\n" << std::endl;
  }
}