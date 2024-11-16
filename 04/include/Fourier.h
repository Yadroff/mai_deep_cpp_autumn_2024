#pragma once

#include <complex>
#include <vector>

#define all(v) v.begin(), v.end()

class Fourier {
public:
  using Complex = std::complex<double>;

  static void fft(std::vector<Complex> &, bool invert);

  static void multiply(const std::vector<int> &lhs, const std::vector<int> &rhs,
                       std::vector<unsigned long long> &res);
};
