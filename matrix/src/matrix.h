#pragma once

#include <iostream>
#include <vector>

namespace task {

const double EPS = 1e-6;

class OutOfBoundsException : public std::exception {};
class SizeMismatchException : public std::exception {};

class Matrix {
public:
  class Row;

  size_t num_cols;
  size_t num_rows;
  Row *rows;

  class Row {

  public:
    double *row;

    Row(){};

    Row(size_t size) {
      row = new double[size];
      for (size_t i = 0; i < size; ++i)
        row[i] = 0;
    }

    double &operator[](size_t col_num) { return row[col_num]; }

    double &operator[](size_t col_num) const { return row[col_num]; }
  };

  Matrix();
  Matrix(size_t num_rows, size_t num_cols);
  Matrix(const Matrix &copy);
  Matrix &operator=(const Matrix &a);

  double &get(size_t row, size_t col);
  const double &get(size_t row, size_t col) const;
  void set(size_t row, size_t col, const double &value);
  void resize(size_t new_num_rows, size_t new_num_cols);

  Row &operator[](size_t row_num);

  Row &operator[](size_t row_num) const;

  Matrix &operator+=(const Matrix &a);
  Matrix &operator-=(const Matrix &a);
  Matrix &operator*=(const Matrix &a);
  Matrix &operator*=(const double &number);

  Matrix operator+(const Matrix &a) const;
  Matrix operator-(const Matrix &a) const;
  Matrix operator*(const Matrix &a) const;
  Matrix operator*(const double &a) const;

  Matrix operator-() const;
  Matrix operator+() const;

  double det() const;
  void transpose();
  Matrix transposed() const;
  double trace() const;

  std::vector<double> getRow(size_t row);
  std::vector<double> getColumn(size_t col);

  bool operator==(const Matrix &a) const;
  bool operator!=(const Matrix &a) const;
};

Matrix operator*(const double &a, const Matrix &b);

std::ostream &operator<<(std::ostream &output, const Matrix &matrix);

std::istream &operator>>(std::istream &input, Matrix &matrix);

} // namespace task
