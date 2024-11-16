#include "Fourier.h"

void Fourier::fft(std::vector<Complex> &a, bool invert) {
    int n = (int) a.size();
    if (n == 1) return;

    std::vector<Complex> a0(n / 2), a1(n / 2);
    for (int i = 0, j = 0; i < n; i += 2, ++j) {
        a0[j] = a[i];
        a1[j] = a[i + 1];
    }
    fft(a0, invert);
    fft(a1, invert);

    double ang = 2 * M_PI / n * (invert ? -1 : 1);
    Complex w(1), wn(std::cos(ang), std::sin(ang));
    for (int i = 0; i < n / 2; ++i) {
        a[i] = a0[i] + w * a1[i];
        a[i + n / 2] = a0[i] - w * a1[i];
        if (invert) {
            a[i] /= 2;
            a[i + n / 2] /= 2;
        }
        w *= wn;
    }
}

void Fourier::multiply(const std::vector<int> &a, const std::vector<int> &b,
                       std::vector<unsigned long long> &res) {
    std::vector<Complex> fa(all(a)), fb(all(b));
    size_t n = 1;
    while (n < std::max(fa.size(), fb.size())) {
        n <<= 1;
    }
    n <<= 1;
    fa.resize(n);
    fb.resize(n);
    fft(fa, false);
    fft(fb, false);
    for (size_t i = 0; i < n; ++i) {
        fa[i] *= fb[i];
    }
    fft(fa, true);
    res.resize(n);
    for (size_t i = 0; i < n; ++i) {
        res[i] = (unsigned long long) (fa[i].real() + 0.5);
    }
}
