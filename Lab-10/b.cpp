#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <cstring>
#include <queue>
#include <stack>
// #pragma GCC optimize("O3")
using namespace std;
using namespace std::chrono;
using ll = long long;

constexpr int maxn = 201;
constexpr int maxm = 100 * 199 + 200;

vector<int> edges[maxn]; // indices of out-edges
int prev[maxn];          // the edge
int srcs[maxm * 2];
int dsts[maxm * 2];
int flows[maxm * 2];
bool visited[maxn];
bool isgirl[maxn];
int n, m, k, s, t, total, girlcnt;
vector<int> pending;

bool augment1(int source)
{
    for (int i = 0; i < n; ++i)
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
    int c = t;
    while (c != s)
    {
        int edg = ::prev[c];
        flows[edg] -= 1;
        flows[edg ^ 1] += 1;
        c = srcs[edg];
    }
    return true;
}

void dfs(int source)
{
    // cout << "visiting " << source << "\n";
    visited[source] = true;
    pending.push_back(source);
    for (int i = 0; i < edges[source].size(); ++i)
    {
        int edg = edges[source][i];
        int dst = dsts[edg];
        if (visited[dst] || flows[edg] == 0)
            continue;
        if (isgirl[dst])
            ++girlcnt;
        dfs(dst);
    }
    // cout << "\tback from visiting " << source << "\n";
}

int solve0()
{
    for (int i = 0; i < n; ++i)
        visited[i] = false;
    dfs(0);
    int edgcnt = 0;
    for (int i = 0; i < pending.size(); ++i)
    {
        int k = pending[i];
        for (int i = 0; i < edges[k].size(); ++i)
        {
            int edg = edges[k][i];
            int dst = dsts[edg];
            if ((edg & 1) == 0 && dst != t && !visited[dst])
                ++edgcnt;
        }
    }
    // for (int i : pending)
    //     cout << i << "\n";
    // cout << edgcnt << ", " << girlcnt << "\n";
    // cout << "flows\n";
    // for (int i = 0; i < total; i += 2)
    //     cout << srcs[i] << "->" << dsts[i] << ", " << flows[i] << "\n";
    // cout << "flows, end\n";
    return edgcnt + girlcnt;
}

void solve()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    cin >> n >> m >> k;
    int *girls = new int[k];
    for (int i = 0, t; i < k; ++i)
        cin >> girls[i], isgirl[girls[i]] = true;
    for (int i = 0; i < m; ++i)
    {
        int u, v;
        cin >> u >> v;
        srcs[total] = u;
        dsts[total] = v;
        flows[total] = 1;
        srcs[total + 1] = v;
        dsts[total + 1] = u;
        edges[u].push_back(total);
        edges[v].push_back(total + 1);
        total += 2;
        srcs[total] = v;
        dsts[total] = u;
        flows[total] = 1;
        srcs[total + 1] = u;
        dsts[total + 1] = v;
        edges[v].push_back(total);
        edges[u].push_back(total + 1);
        total += 2;
    }
    for (int i = 0; i < k; ++i)
    {
        srcs[total] = girls[i];
        dsts[total] = n;
        flows[total] = 1;
        srcs[total + 1] = n;
        dsts[total + 1] = girls[i];
        edges[girls[i]].push_back(total);
        edges[n].push_back(total + 1);
        total += 2;
    }
    t = n;
    n += 1;
    s = 0;

    while (augment())
        ;

    cout << solve0() << "\n";

    // cout << "sadsada:\n";
    // for (int i = 0; i < total; ++i)
    //     cout << "edge " << i << " ,src=" << srcs[i] << ", dst=" << dsts[i] << "\n";
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