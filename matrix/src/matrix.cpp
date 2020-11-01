#include "matrix.h"

using namespace task;

Matrix::Matrix() : num_cols(1), num_rows(1) {
  rows = new Matrix::Row[num_rows];
  for (size_t i = 0; i < num_rows; ++i)
    rows[i] = Matrix::Row(num_cols);

  rows[0][0] = 1.;
}

Matrix::Matrix(size_t num_rows, size_t num_cols)
    : num_rows(num_rows), num_cols(num_cols) {
  rows = new Matrix::Row[num_rows];
  for (size_t i = 0; i < num_rows; ++i)
    rows[i] = Matrix::Row(num_cols);

  for (size_t i = 0; i < num_rows; ++i)
    for (size_t j = 0; j < num_cols; ++j)
      if (i == j)
        rows[i][j] = 1.;
}

Matrix::Matrix(const Matrix &copy)
    : num_cols(copy.num_cols), num_rows(copy.num_rows) {
  rows = new Matrix::Row[num_rows];
  for (size_t i = 0; i < num_rows; ++i)
    rows[i] = Matrix::Row(num_cols);

  for (size_t i = 0; i < num_rows; ++i)
    for (size_t j = 0; j < num_cols; ++j)
      rows[i][j] = copy.rows[i][j];
}

Matrix &Matrix::operator=(const Matrix &a) {
  Row *new_rows = new Matrix::Row[a.num_rows];
  for (size_t i = 0; i < a.num_rows; ++i)
    new_rows[i] = Matrix::Row(a.num_cols);

  for (size_t i = 0; i < a.num_rows; ++i)
    for (size_t j = 0; j < a.num_cols; ++j)
      new_rows[i][j] = a.rows[i][j];

  for (size_t i = 0; i < num_rows; ++i)
    delete[] rows[i].row;

  delete[] rows;

  num_cols = a.num_cols;
  num_rows = a.num_rows;

  rows = new_rows;

  return *this;
}

Matrix::Row &Matrix::operator[](size_t row_num) { return rows[row_num]; }

Matrix::Row &Matrix::operator[](size_t row_num) const { return rows[row_num]; }

double &Matrix::get(size_t row, size_t col) {
  if (0 <= row && row < num_rows && 0 <= col && col < num_cols)
    return rows[row][col];

  throw OutOfBoundsException();
}

const double &Matrix::get(size_t row, size_t col) const {
  if (0 <= row && row < num_rows && 0 <= col && col < num_cols)
    return rows[row][col];

  throw OutOfBoundsException();
}

void Matrix::set(size_t row, size_t col, const double &value) {
  if (0 <= row && row < num_rows && 0 <= col && col < num_cols)
    rows[row][col] = value;
  else
    throw OutOfBoundsException();
}

void Matrix::resize(size_t new_num_rows, size_t new_num_cols) {
  Row *new_rows = new Matrix::Row[new_num_rows];
  for (size_t i = 0; i < new_num_rows; ++i)
    new_rows[i] = Matrix::Row(new_num_cols);

  for (size_t i = 0; i < std::min(num_rows, new_num_rows); ++i)
    for (size_t j = 0; j < std::min(num_cols, new_num_cols); ++j)
      new_rows[i][j] = rows[i][j];

  for (size_t i = 0; i < num_rows; ++i)
    delete[] rows[i].row;

  delete[] rows;

  num_cols = new_num_cols;
  num_rows = new_num_rows;

  rows = new_rows;
}

std::vector<double> Matrix::getRow(size_t row) {
  std::vector<double> row_vec;
  for (size_t i = 0; i < num_cols; ++i)
    row_vec.push_back(rows[row][i]);

  return row_vec;
}

std::vector<double> Matrix::getColumn(size_t col) {
  std::vector<double> col_vec;
  for (size_t i = 0; i < num_rows; ++i)
    col_vec.push_back(rows[i][col]);

  return col_vec;
}

Matrix &Matrix::operator*=(const double &number) {
  for (size_t i = 0; i < num_rows; ++i)
    for (size_t j = 0; j < num_cols; ++j)
      rows[i][j] *= number;

  return *this;
}

Matrix Matrix::operator*(const double &a) const {
  Matrix new_mat(*this);
  new_mat *= a;
  return new_mat;
}

Matrix Matrix::operator*(const Matrix &a) const {
  if (this->num_rows == a.num_cols) {
    Matrix new_mat(this->num_rows, a.num_cols);
    for (size_t row_i = 0; row_i < new_mat.num_rows; ++row_i)
      for (size_t col_j = 0; col_j < new_mat.num_cols; ++col_j) {
        double res_i_j = 0;
        for (size_t i = 0; i < a.num_cols; ++i)
          res_i_j += (this->rows[row_i][i] * a.rows[i][col_j]);
        new_mat[row_i][col_j] = res_i_j;
      }

    return new_mat;
  }

  throw SizeMismatchException();
}

Matrix &Matrix::operator*=(const Matrix &a) {
  Matrix tmp(*this);
  *this = tmp * a;

  return *this;
}

Matrix &Matrix::operator+=(const Matrix &a) {
  if (this->num_rows == a.num_rows && this->num_cols == a.num_cols) {
    for (size_t i = 0; i < this->num_rows; ++i)
      for (size_t j = 0; j < this->num_cols; ++j)
        (*this)[i][j] += a[i][j];

    return *this;
  }

  throw SizeMismatchException();
}

Matrix &Matrix::operator-=(const Matrix &a) {
  if (this->num_rows == a.num_rows && this->num_cols == a.num_cols) {
    for (size_t i = 0; i < this->num_rows; ++i)
      for (size_t j = 0; j < this->num_cols; ++j)
        (*this)[i][j] -= a[i][j];

    return *this;
  }

  throw SizeMismatchException();
}

Matrix Matrix::operator+(const Matrix &a) const {
  Matrix new_mat(*this);
  new_mat += a;
  return new_mat;
}

Matrix Matrix::operator-(const Matrix &a) const {
  Matrix new_mat(*this);
  new_mat -= a;
  return new_mat;
}

Matrix Matrix::operator-() const {
  Matrix new_mat(*this);
  new_mat *= -1;

  return new_mat;
}

Matrix Matrix::operator+() const {
  Matrix new_mat(*this);

  return new_mat;
}

double custom_abs(const double &a, const double &b) {
  if (a > b)
    return a - b;
  else
    return b - a;
}

bool Matrix::operator==(const Matrix &a) const {
  if (this->num_rows == a.num_rows && this->num_cols == a.num_cols) {
    for (size_t i = 0; i < this->num_rows; ++i)
      for (size_t j = 0; j < this->num_cols; ++j) {
        if (custom_abs(this->rows[i][j], a.rows[i][j]) >= EPS)
          return false;
      }
  }

  return true;

  throw SizeMismatchException();
}

bool Matrix::operator!=(const Matrix &a) const { return !(*this == a); }

std::ostream &task::operator<<(std::ostream &output, const Matrix &matrix) {
  for (size_t i = 0; i < matrix.num_rows; ++i) {
    for (size_t j = 0; j < matrix.num_cols; ++j)
      output << matrix[i][j] << " ";

    output << '\n';
  }

  output << '\n';

  return output;
}

std::istream &task::operator>>(std::istream &input, Matrix &matrix) {
  size_t num_rows, num_cols;
  input >> num_rows >> num_cols;
  for (size_t i = 0; i < matrix.num_rows; ++i)
    for (size_t j = 0; j < matrix.num_cols; ++j) {
      double temp;
      input >> temp;
      matrix[i][j] = temp;
    }

  return input;
}

Matrix build_minor(const Matrix &mat, const size_t &row, const size_t &col) {
  Matrix tmp(mat.num_rows - 1, mat.num_cols - 1); // rows == cols
  size_t idx_row = 0;
  size_t idx_col = 0;
  for (size_t i = 0; i < mat.num_rows; ++i) {
    if (i == row)
      continue;

    for (size_t j = 0; j < mat.num_cols; ++j) {
      if (j == col)
        continue;

      tmp[idx_row][idx_col] = mat[i][j];
      ++idx_col;
    }

    idx_col = 0;
    ++idx_row;
  }

  return tmp;
}

int get_sign(size_t x) { return (x % 2 == 0) ? 1 : -1; }

double _det(const Matrix &a) {
  double res_det = 0.;

  if (a.num_cols == 1)
    return a[0][0];

  if (a.num_cols == 2)
    return a[0][0] * a[1][1] - a[0][1] * a[1][0];

  for (size_t i = 0; i < a.num_cols; ++i) {
    res_det += get_sign(i) * a[0][i] * _det(build_minor(a, 0, i));
  }

  return res_det;
}

double Matrix::det() const {
  if (this->num_rows != this->num_cols)
    throw SizeMismatchException();

  return _det(*this);
}

Matrix Matrix::transposed() const {
  Matrix tmp(this->num_cols, this->num_rows);

  for (size_t i = 0; i < this->num_rows; ++i)
    for (size_t j = 0; j < this->num_cols; ++j)
      tmp[j][i] = (*this)[i][j];

  return tmp;
}

void Matrix::transpose() { *this = (*this).transposed(); }

double Matrix::trace() const {
  if (this->num_cols != this->num_rows)
    throw SizeMismatchException();

  double res_trace = 0.;
  for (size_t i = 0; i < this->num_cols; ++i)
    res_trace += (*this)[i][i];

  return res_trace;
}

Matrix task::operator*(const double &a, const Matrix &b) {
  Matrix tmp(b);
  for (size_t i = 0; i < tmp.num_rows; ++i)
    for (size_t j = 0; j < tmp.num_cols; ++j)
      tmp[i][j] *= a;

  return tmp;
}