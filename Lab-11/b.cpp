#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <cstring>
// #include <queue>
// #include <stack>
// #pragma GCC optimize("O3")
using namespace std;
using namespace chrono;
using ll = long long;

constexpr ll mod = 1'000'000'000 + 7;

ll function0(int base, ll exp)
{
    ll ans = 1, tmp = base;
    ll exp0 = exp;
    while (exp0)
    {
        if (exp0 & 1) ans = (ans * tmp) % mod;
        tmp = (tmp * tmp) % mod;
        exp0 >>= 1;
    }
    return ans;
}

ll function1(ll divisor, ll dividend)
{
    ll inv = function0(dividend, mod - 2);
    // cout << inv << endl;
    ll ans = divisor;
    ans = (ans * inv) % mod;
    return ans;
}

void solve()
{
    // cin.tie(nullptr);
    // cin.sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    int ans = function0(2 * (n + 1), m);
    ans = function1(((ll)ans * (n + 1 - m)) % mod, n + 1);
    cout << ans << endl;
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