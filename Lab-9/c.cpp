#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <cstring>
// #pragma GCC optimize("O3")
using namespace std;
using namespace std::chrono;
using ll = long long;

constexpr int maxn = 500;
constexpr int aub1 = maxn + 2;
ll n, p;
ll dp[aub1][aub1], dp1[aub1][aub1], dp2[aub1][aub1];
ll base[] = {2, 3, 5, 7, 11, 13, 17, 19};

struct struct1
{
    ll rem_factor = 0, bit_repr = 0;
} nodes[aub1];

void solve()
{
    cin >> n >> p;
    // preprocess numbers
    for (int i = 2; i <= n; ++i)
    {
        ll r = i;
        for (int j = 0; j < 8; ++j)
        {
            if (r % base[j] == 0)
            {
                nodes[i].bit_repr |= (1 << j);
                while (r % base[j] == 0)
                    r /= base[j];
            }
        }
        if (r != 1)
            nodes[i].rem_factor = r;
    }
    sort(nodes + 2, nodes + n + 1, [](struct1 &lhs, struct1 &rhs)
         { return lhs.rem_factor < rhs.rem_factor; });

    dp[0][0] = 1;
    for (int i = 2; i <= n; ++i) // count each node separately, sorted by their rem_factor
    {
        if (nodes[i].rem_factor == 0 || nodes[i].rem_factor != nodes[i - 1].rem_factor)
        {
            memcpy(dp1, dp, sizeof(dp));
            memcpy(dp2, dp, sizeof(dp));
        }
        for (ll j = 0xffLL; j >= 0; --j) // try to place this number at either left or right.
        {
            for (ll k = 0xffLL; k >= 0; --k)
            {
                if (j & k)
                    continue;
                if ((nodes[i].bit_repr & j) == 0) // no overlapping factor, placing at the right group
                    dp1[j][k | nodes[i].bit_repr] = (dp1[j][k | nodes[i].bit_repr] + dp1[j][k]) % p;
                    // if k == nodes[i].bit_repr, then a self addition will occur
                    
                if ((nodes[i].bit_repr & k) == 0) // no overlapping factor, placing at the left group
                    dp2[j | nodes[i].bit_repr][k] = (dp2[j | nodes[i].bit_repr][k] + dp2[j][k]) % p;
            }
        }
        if (i == n || nodes[i].rem_factor == 0 || nodes[i].rem_factor != nodes[i + 1].rem_factor)
        {
            for (ll j = 0; j <= 0xffLL; ++j)
            {
                for (ll k = 0; k <= 0xffLL; ++k)
                {
                    if (j & k)
                        continue;
                    dp[j][k] = (dp1[j][k] + dp2[j][k] - dp[j][k] + p) % p; // merge
                }
            }
        }
    }

    ll ans = 0;
    for (int i = 0; i <= 0xffll; ++i)
    {
        for (int j = 0; j <= 0xffll; ++j)
        {
            if (i & j)
                continue;
            ans = (ans + dp[i][j]) % p;
        }
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