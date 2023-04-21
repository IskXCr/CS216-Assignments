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

constexpr int N = 4000 + 1, N = 4000 + 1;
// f - dp array
// v - value
// w - weight
// s - number of available items
// q - the monotonic queue that stores the "dynamic" part in descending order
int f[N][N], w[N], v[N], s[N], q[N];
int n, m;

void solve()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);
    cin >> n >> m;
    for (int i = 1; i <= n; ++i)
        cin >> v[i] >> w[i] >> s[i];

    for (int i = 1; i <= n; ++i)
    {
        for (int r = 0; r < w[i]; ++r)
        {
            // refresh the queue
            int head = 0, tail = -1;
            for (int k = 0; r + k * w[i] <= m; ++k)
            {
                // remove the out-of-range one
                if (head <= tail && (k - q[head]) > s[i])
                    ++head;
                // update the queue
                while (head <= tail && (f[i - 1][r + q[tail] * w[i]] - q[tail] * v[i]) < (f[i - 1][r + k * w[i]] - k * v[i]))
                    --tail;
                q[++tail] = k;
                f[i][r + k * w[i]] = f[i - 1][r + q[head] * w[i]] + (k - q[head]) * v[i];
            }
        }
    }

    cout << f[n][m] << endl;
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