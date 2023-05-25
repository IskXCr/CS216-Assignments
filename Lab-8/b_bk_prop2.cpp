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

constexpr int mod = 12345678;
constexpr int N = 150, M = 150, K = 20;

int dp[N + 1][M + 1][K + 1][K + 1];

void back_propagate(int n, int m, int k)
{
    dp[0][0][0][0] = 1;
    for (int a = 0; a <= n; ++a)
    {
        for (int b = 0; b <= m; ++b)
        {
            for (int c = 0; c <= k; ++c)
            {
                for (int d = 0; d <= k; ++d)
                {
                    if (dp[a][b][c][d] != 0)
                    {
                        if (a < n && c < k)
                            dp[a + 1][b][c + 1][max(d - 1, 0)] = (dp[a + 1][b][c + 1][max(d - 1, 0)] + dp[a][b][c][d]) % mod;
                        if (b < m && d < k)
                            dp[a][b + 1][max(c - 1, 0)][d + 1] = (dp[a][b + 1][max(c - 1, 0)][d + 1] + dp[a][b][c][d]) % mod;
                    }
                }
            }
        }
    }
}

int accumulate(int n, int m, int k)
{
    int ans = 0;
    for (int c = 0; c <= k; ++c)
    {
        for (int d = 0; d <= k; ++d)
        {
            ans = (ans + dp[n][m][c][d]) % mod;
        }
    }
    return ans;
}

void solve()
{
    int n, m, k;
    cin >> n >> m >> k;
    back_propagate(n, m, k);
    cout << accumulate(n, m, k) << "\n";
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