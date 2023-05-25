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
constexpr int SZ = 500 + 2;
char buffer[SZ + 1];
int dp[SZ][SZ];
int n;

int solve(int l, int r)
{
    if (l > r)
        return 0;
    if (l == r)
        return 1;
    if (dp[l][r] != -1)
        return dp[l][r];

    int res = 1 + solve(l + 1, r);
    for (int i = l + 1; i <= r; ++i)
        if (buffer[l] == buffer[i])
            res = min(res, solve(l + 1, i - 1) + solve(i, r));
    return dp[l][r] = res;
}

void solve()
{
    memset(dp, 0xff, sizeof dp);
    scanf("%d%s", &n, buffer + 1);
    printf("%d\n", solve(1, n));
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