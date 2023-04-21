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
        cin >> s[i] >> w[i] >> v[i];
    for (int i = 1; i <= n; ++i) // iterating through items
    {
        for (int x = 0; x < w[i]; ++x) // iterating through the set of remainders of w[i] (update the array categorized by remainder)
        {
            int head = 0, tail = -1;
            for (int j = x; j <= m; j += w[i])
            {
                if (head <= tail && q[head] < j - s[i] * w[i]) // pop the front item that is out-of-range. This needs to be done only once per iteration.
                    ++head;
                while (head <= tail && (f[i - 1][q[tail]] - (q[tail] - x) / w[i] * v[i]) < (f[i - 1][j] - (j - x) / w[i] * v[i])) // update the monotonic queue
                    --tail;
                q[++tail] = j;                                               // insert into the monotonic queue
                f[i][j] = f[i - 1][q[head]] + ((j - q[head]) / w[i]) * v[i]; // select the maximum one
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