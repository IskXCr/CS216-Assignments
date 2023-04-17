#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <cstring>
#pragma GCC optimize("O3")
using namespace std;
using namespace std::chrono;

using ll = long long;

struct complex
{
    double a;
    double b;

    complex() : a{0}, b{0} {}
    complex(double a, double b) : a{a}, b{b} {}

    complex operator-() const
    {
        return complex(-this->a, -this->b);
    }

    inline complex &operator*=(const complex &other)
    {
        double d1, d2;
        d1 = this->a * other.a - this->b * other.b;
        d2 = this->a * other.b + this->b * other.a;
        this->a = d1;
        this->b = d2;
        return *this;
    }

    inline complex &operator*=(const double other)
    {
        this->a *= other;
        this->b *= other;
        return *this;
    }

    inline friend complex operator+(const complex &lhs, const complex &rhs)
    {
        return complex(lhs.a + rhs.a, lhs.b + rhs.b);
    }

    inline friend complex operator-(const complex &lhs, const complex &rhs)
    {
        return complex(lhs.a - rhs.a, lhs.b - rhs.b);
    }

    inline complex &operator/=(double f)
    {
        this->a /= f;
        this->b /= f;
        return *this;
    }

    inline friend complex operator/(const complex &lhs, const double rhs)
    {
        complex res = lhs;
        res /= rhs;
        return res;
    }

    inline friend complex operator*(const complex &lhs, const complex &rhs)
    {
        complex res = lhs;
        res *= rhs;
        return res;
    }

    inline friend complex operator*(const complex &lhs, const double rhs)
    {
        complex res = lhs;
        res *= rhs;
        return res;
    }

    inline friend complex operator*(const double lhs, const complex &rhs)
    {
        return rhs * lhs;
    }

    inline friend ostream &operator<<(ostream &os, const complex &c)
    {
        return os << "(" << c.a << "," << c.b << ")";
    }
};

// Evaluate the polynomial at n-th roots of unity
int *fft_pos = nullptr;

// n = power of 2, l = index of the leftmost nonzero digit
void fft_preprocess(int n, int l)
{
    if (fft_pos != nullptr)
        delete[] fft_pos;
    fft_pos = new int[n];
    fft_pos[0] = 0;
    for (int i = 0; i < n; ++i)
        fft_pos[i] = (fft_pos[i >> 1] >> 1) | ((i & 1) << (l - 1));
}

// Do fft on target array. Size must be a power of 2. Store the result directly inside the array. Preprocessing must be done before called.
void fft(int n, complex *coeff, double fft_sign)
{
    for (int i = 0; i < n; ++i)
        if (i < fft_pos[i])
            swap(coeff[i], coeff[fft_pos[i]]);

    for (int mid = 1; mid < n; mid <<= 1) // mid
    {
        double comp = (fft_sign * M_PI) / mid;
        complex w_comp(cos(comp), sin(comp));
        for (int left = 0, step = mid << 1; left < n; left += step) // base offset
        {
            complex w_k(1, 0);
            for (int k = 0; k < mid; k++, w_k = w_k * w_comp)
            {
                complex x = coeff[left + k], y = w_k * coeff[left + mid + k];
                coeff[left + k] = x + y;
                coeff[left + mid + k] = x - y;
            }
        }
    }
    if (fft_sign < 0.0)
    {
        for (int i = 0; i < n; ++i)
            coeff[i] /= n;
    }
}

constexpr int maxlen = 5e5 + 1;
int result[maxlen] = {0};

void solve()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);
    string s;
    cin >> s;
    int len = s.length(), t = 1, l = 0;
    while (t < 2 * len - 1)
        t <<= 1, ++l;
    fft_preprocess(t, l);
    complex *fl = new complex[t];
    complex *flrev = new complex[t];
    complex *fq = new complex[t];
    complex *fqrev = new complex[t];
    complex *fc = new complex[t];
    complex *fcrev = new complex[t];
    complex *res = new complex[t];
    for (int i = 0, j = len - 1; i < len; ++i, --j)
    {
        if (s[i] == '?')
            continue;
        fl[i].a = (s[i] - '0' + 1);
        flrev[j].a = fl[i].a;

        fq[i].a = fl[i].a * fl[i].a;
        fqrev[j].a = fq[i].a;

        fc[i].a = fl[i].a * fl[i].a * fl[i].a;
        fcrev[j].a = fc[i].a;
    }
    fft(t, fl, 1.0);
    fft(t, flrev, 1.0);
    fft(t, fq, 1.0);
    fft(t, fqrev, 1.0);
    fft(t, fc, 1.0);
    fft(t, fcrev, 1.0);
    for (int i = 0; i < t; ++i)
    {
        res[i] = fc[i] * flrev[i] + fl[i] * fcrev[i] - 2 * fq[i] * fqrev[i];
    }
    fft(t, res, -1.0);

    ll ans = 0;

    for (int i = 0, j = 1; i < len / 2; ++i, ++j)
    {
        if (fabs(round(res[i].a)) < 1e-6)
            ans ^= j * (ll)j;
    }
    cout << ans << "\n";
}

decltype(high_resolution_clock::now()) t1;

inline void set_time()
{
    t1 = high_resolution_clock::now();
}

inline void show_time()
{
    std::cout << "[T] " << duration_cast<duration<double>>(high_resolution_clock::now() - t1).count() << " s\n";
}

int main()
{
    // set_time();
    solve();
    // show_time();
    return 0;
}