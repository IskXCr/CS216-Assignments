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
using namespace std::chrono;

constexpr int maxn = 1050;
double dp[maxn + 1][maxn + 1];
int perm[maxn + 1];
int n, m;

void solve()
{
    // cin.tie(nullptr);
    // cin.sync_with_stdio(false);
    cin >> n >> m;
    for (int i = 1; i <= n; ++i)
        cin >> perm[i];
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            dp[i][j] = perm[i] > perm[j];
    
    for (int k = 0; k < m; ++k)
    {
        int a, b;
        cin >> a >> b;
        for (int i = 1; i <= n; ++i)
        {
            dp[a][i] = dp[b][i] = 0.5 * (dp[a][i] + dp[b][i]);
            dp[i][a] = dp[i][b] = 1.0 - dp[a][i];
        }
        dp[a][b] = dp[b][a] = 0.5;
    }

    double ans = 0.0;
    for (int i = 1; i <= n; ++i)
        for (int j = i + 1; j <= n; ++j)
            ans += dp[i][j];
    
    cout << setprecision(9) << fixed << ans;
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