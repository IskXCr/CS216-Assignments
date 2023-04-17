#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <cstring>

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

    inline friend complex operator*(const complex &lhs, const complex &rhs)
    {
        complex res = lhs;
        res *= rhs;
        return res;
    }

    inline friend ostream &operator<<(ostream &os, const complex &c)
    {
        return os << "(" << c.a << "," << c.b << ")";
    }
};

// evaluate the polynomial at n-th roots of unity
int *fft_pos = nullptr;

void fft_preprocess(int t, int l)
{
    if (fft_pos != nullptr)
        delete[] fft_pos;
    fft_pos = new int[t];
    fft_pos[0] = 0;
    for (int i = 0; i < t; ++i)
        fft_pos[i] = (fft_pos[i >> 1] >> 1) | ((i & 1) << (l - 1));
}

// do fft on target array. Size must be a power of 2. Store the result directly inside the array.
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
}

void solve()
{
    int n, m, t, l;
    cin >> n;
    m = 2 * n; // number of items on each polynomial
    t = 1, l = 0;
    while (t <= 2 * m - 1)
        t <<= 1, ++l;
    complex *comp1 = new complex[t];
    complex *comp2 = new complex[t];
    for (int i = 0; i < n; ++i)
        cin >> comp1[i].a;
    double u = n - 1;
    for (int i = 0; i < n - 1; i++, u -= 1.0)
        comp2[i].a = -1.0 / (u * u);
    for (int i = n; i < 2 * n - 1; ++i)
        comp2[i].a = -comp2[2 * n - i - 2].a;
    fft_preprocess(t, l);
    fft(t, comp1, 1.0);
    fft(t, comp2, 1.0);
    for (int i = 0; i < t; ++i)
        comp1[i] *= comp2[i];
    fft(t, comp1, -1.0);
    for (int i = 0; i < t; ++i)
        comp1[i] /= t;
    cout << fixed << setprecision(3);
    for (int i = n - 1; i < 2 * n - 1; ++i)
    {
        cout << comp1[i].a << "\n";
    }
}

int main()
{
    // auto t1 = high_resolution_clock::now();
    cin.tie(nullptr);
    cin.sync_with_stdio(false);
    solve();
    // auto t2 = high_resolution_clock::now();
    // std::cout << "[T] Took " << duration_cast<duration<double>>(t2 - t1).count() << " s\n";
    return 0;
}