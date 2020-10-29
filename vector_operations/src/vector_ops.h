#pragma once
#include <iostream>
#include <vector>

namespace task {
std::vector<double> operator+(const std::vector<double> &a);

std::vector<double> operator-(const std::vector<double> &a);

std::vector<double> operator+(const std::vector<double> &a,
                              const std::vector<double> &b);

std::vector<double> operator-(const std::vector<double> &a,
                              const std::vector<double> &b);

double operator*(const std::vector<double> &a, const std::vector<double> &b);

std::vector<double> operator%(const std::vector<double> &a,
                              const std::vector<double> &b);

bool operator||(const std::vector<double> &a, const std::vector<double> &b);

bool operator&&(const std::vector<double> &a, const std::vector<double> &b);

std::istream &operator>>(std::istream &is, std::vector<double> &a);

std::ostream &operator<<(std::ostream &os, const std::vector<double> &a);

void reverse(std::vector<double> &a);

std::vector<int> operator|(const std::vector<int> &a,
                           const std::vector<int> &b);

std::vector<int> operator&(const std::vector<int> &a,
                           const std::vector<int> &b);
} // namespace task
