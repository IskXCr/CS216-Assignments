#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <cstring>
#include <queue>
// #pragma GCC optimize("O3")
using namespace std;
using namespace std::chrono;
using ll = long long;

constexpr int maxn = 201;
constexpr int maxm = 5001;

vector<int> edges[maxn]; // indices of out-edges
int flag[maxn][maxn];
int prev[maxn]; // the edge
int srcs[maxm * 2];
int dsts[maxm * 2];
ll flows[maxm * 2];
ll capacities[maxm * 2];
bool visited[maxn];
ll ans = 0;

int n, m, s, t, total;

bool augment1(int source)
{
    for (int i = 1; i <= n; ++i)
        visited[i] = false;

    queue<int> q;
    q.push(source);
    visited[source] = true;

    while (!q.empty())
    {
        int k = q.front();
        q.pop();
        for (int i = 0; i < edges[k].size(); ++i)
        {
            int edg = edges[k][i];
            int dst = dsts[edg];
            if (flows[edg] == 0 || visited[dst])
                continue;
            ::prev[dst] = edg;
            q.push(dst);
            visited[dst] = true;
            if (dst == t)
                return true;
        }
    }
    return false;
}

bool augment()
{
    if (!augment1(s))
        return false;
    ll rem = numeric_limits<ll>().max();
    int c = t;
    while (c != s)
    {
        int edg = ::prev[c];
        rem = min(rem, flows[edg]);
        c = srcs[edg];
    }
    c = t;
    while (c != s)
    {
        int edg = ::prev[c];
        flows[edg] -= rem;
        if (edg > m)
            flows[edg - m] += rem;
        else
            flows[edg + m] += rem;
        c = srcs[edg];
    }
    ans += rem;
    return true;
}

void solve()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    cin >> n >> m >> s >> t;
    for (int i = 0; i < m; ++i)
    {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        if (flag[u][v])
        {
            flows[flag[u][v]] += w;
        }
        else
        {
            ++total;
            flag[u][v] = total;
            srcs[total] = u;
            dsts[total] = v;
            flows[total] = w;
            srcs[total + m] = v;
            dsts[total + m] = u;
            edges[u].push_back(total);
            edges[v].push_back(total + m);
        }
    }

    while (augment())
        ;
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
    set_time();
    solve();
    // show_time();
    return 0;
}