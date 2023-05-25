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

constexpr int maxn = 7000, maxm = 20000;

vector<int> nodes[maxn + 1];
int parent[maxn + 1], visited[maxn + 1];
double dist[maxn + 1];
int esrc[maxm], edst[maxm], ev[maxm], ep[maxm]; 
double ecost[maxm];

int n, m;

void f1(double ans)
{
    for (int i = 0; i < m; ++i)
        ecost[i] = ans * ep[i] - ev[i];
    memset(parent, 0, (n + 1) * sizeof(int));
    memset(visited, 0, (n + 1) * sizeof(int));
    for (int i = 1; i <= n; ++i)
        dist[i] = numeric_limits<double>().max();
    dist[0] = 0;
}

bool f2(int src)
{
    visited[src] = 1;

    for (int i = 0; i < nodes[src].size(); ++i)
    {
        int idx = nodes[src][i];
        if (dist[esrc[idx]] + ecost[idx] < dist[edst[idx]]) // if higher, do update.
        {
            if (visited[edst[idx]])
            {
                return false;
            }
            else
            {
                dist[edst[idx]] = dist[esrc[idx]] + ecost[idx];
                if (!f2(edst[idx]))
                    return false;
            }
        }
    }

    visited[src] = 0;
    return true;
}

void solve()
{
    cin >> n >> m;
    // spaces
    for (int i = 0; i < m; ++i)
        cin >> esrc[i] >> edst[i] >> ev[i];
    for (int i = 0; i < m; ++i)
        cin >> ep[i];

    // luogu
    // for (int i = 0; i < m; ++i)
    //     cin >> esrc[i] >> edst[i] >> ev[i] >> ep[i];

    for (int i = m, j = 1; i < m + n; ++i, ++j)
        esrc[i] = 0, edst[i] = j, ev[i] = 0, ep[i] = 0;
    
    // make graph
    for (int i = 0; i < m + n; ++i)
        nodes[esrc[i]].push_back(i);

    double l = 0, r = 200;
    while (l + 0.00001 < r)
    {
        double mid = (l + r) / 2;
        f1(mid);
        if (f2(0))
            r = mid;
        else
            l = mid;
    }
    if (l == 0)
        cout << -1 << "\n";
    else
        cout << setprecision(1) << fixed << l << "\n";
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