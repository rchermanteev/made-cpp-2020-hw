#include "vector_ops.h"

using namespace std;

vector<double> task::operator+(const vector<double> &a) { return a; }

vector<double> task::operator-(const vector<double> &a) {
  vector<double> res_vec;
  res_vec.resize(a.size());
  for (size_t i = 0; i < a.size(); ++i)
    res_vec[i] = a[i] * -1;

  return res_vec;
}

vector<double> task::operator+(const vector<double> &a,
                               const vector<double> &b) {
  vector<double> res_vec;
  res_vec.resize(a.size());
  for (size_t i = 0; i < a.size(); ++i)
    res_vec[i] = a[i] + b[i];

  return res_vec;
}

vector<double> task::operator-(const vector<double> &a,
                               const vector<double> &b) {
  vector<double> res_vec;
  res_vec.resize(a.size());

  for (size_t i = 0; i < a.size(); ++i)
    res_vec[i] = a[i] - b[i];

  return res_vec;
}

double task::operator*(const vector<double> &a, const vector<double> &b) {
  double result = 0;
  for (size_t i = 0; i < a.size(); ++i)
    result += a[i] * b[i];

  return result;
}

vector<double> task::operator%(const vector<double> &a,
                               const vector<double> &b) {
  if (a.size() != 3 || b.size() != 3)
    throw "bad input values";

  vector<double> res_vec;

  res_vec.push_back(a[1] * b[2] - a[2] * b[1]);
  res_vec.push_back(-(a[0] * b[2] - a[2] * b[0]));
  res_vec.push_back(a[0] * b[1] - a[1] * b[0]);

  return res_vec;
}

bool task::operator||(const vector<double> &a, const vector<double> &b) {
  const double EPS = 1e-7;
  size_t i;
  for (i = 0; i < a.size(); ++i)
    if (a[i] != 0 && b[i] != 0)
      break;

  if (i == a.size() || a[i] == 0 || b[i] == 0)
    return false;

  double k = a[i] / b[i];
  while (++i != a.size())
    if (a[i] - k * b[i] > EPS)
      return false;

  return true;
}

bool sign(double x) { return (x >= 0) ? true : false; }

bool task::operator&&(const vector<double> &a, const vector<double> &b) {
  if (a || b) {
    for (size_t i = 0; i < a.size(); ++i)
      if (sign(a[i]) != sign(b[i]))
        return false;

    return true;
  }

  return false;
}

void task::reverse(std::vector<double> &a) {
  size_t last_idx_a = a.size() - 1;
  for (size_t i = 0; i < (a.size() / 2); ++i)
    swap(a[i], a[last_idx_a - i]);
}

vector<int> task::operator|(const vector<int> &a, const vector<int> &b) {
  vector<int> res_vec;
  res_vec.resize(a.size());
  for (size_t i = 0; i < a.size(); ++i)
    res_vec[i] = a[i] | b[i];

  return res_vec;
}

vector<int> task::operator&(const vector<int> &a, const vector<int> &b) {
  vector<int> res_vec;
  res_vec.resize(a.size());
  for (size_t i = 0; i < a.size(); ++i)
    res_vec[i] = a[i] & b[i];

  return res_vec;
}

ostream &task::operator<<(ostream &os, const vector<double> &a) {
  for (size_t i = 0; i < a.size(); ++i)
    os << a[i] << " ";

  os << '\n';

  return os;
}

istream &task::operator>>(istream &is, vector<double> &a) {
  if (a.size() != 0)
    a.clear();

  size_t sz;
  is >> sz;
  for (size_t i = 0; i < sz; ++i) {
    double temp;
    is >> temp;
    a.push_back(temp);
  }

  return is;
}
