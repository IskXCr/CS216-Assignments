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

constexpr int maxn = 10000 + 10000;
constexpr int maxm = 50 * 99 + 10000;
constexpr int maxint = std::numeric_limits<int>().max();

vector<int> edges[maxn]; // indices of out-edges
int prev[maxn];          // the edge
int srcs[maxm * 2];
int dsts[maxm * 2];
int flows[maxm * 2];
int capacities[maxm * 2];
bool visited[maxn];

int n, m, s, t, total;

void addedg(int src, int dst, int flow)
{
    // cout << "adding edge " << total << " from " << src << " to " << dst << " with flow " << flow << "\n";
    srcs[total] = src;
    dsts[total] = dst;
    flows[total] = flow;
    srcs[total + 1] = dst;
    dsts[total + 1] = src;
    edges[src].push_back(total);
    edges[dst].push_back(total + 1);
    total += 2;
}

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
        // cout << "accessing " << k << "\n";
        for (int i = 0; i < edges[k].size(); ++i)
        {
            int edg = edges[k][i];
            // cout << "\taccessing " << edg << ", src=" << srcs[edg] << ", dst=" << dsts[edg] << ", flow=" << flows[edg]  << "\n";
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

int cut;

bool augment()
{
    if (!augment1(s))
        return false;
    int rem = maxint;
    int c = t;
    // cout << "path: " << c;
    while (c != s)
    {
        int edg = ::prev[c];
        rem = min(rem, flows[edg]);
        c = srcs[edg];
        // cout << " <- " << c;
    }
    // cout << "...";
    c = t;
    while (c != s)
    {
        int edg = ::prev[c];
        flows[edg] -= rem;
        flows[edg ^ 1] += rem;
        c = srcs[edg];
    }
    cut += rem;
    // cout << "augmented by " << rem << "\n";
    return true;
}

constexpr int dir[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

inline bool check(int val, int l, int r)
{
    return l <= val && val < r;
}

void solve()
{
    // cin.tie(nullptr);
    // cin.sync_with_stdio(false);
    cin >> m >> n;
    int sum = 0;
    s = 0;
    t = m * n + 1;
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            int w;
            cin >> w;
            sum += w;
            if (((i + j) & 1) == 0)
            {
                addedg(s, i * n + j + 1, w);
                for (int k = 0; k < 4; ++k)
                {
                    int a = i + dir[k][0], b = j + dir[k][1];
                    if (check(a, 0, m) && check(b, 0, n))
                        addedg(i * n + j + 1, a * n + b + 1, maxint);
                }
            }
            else
            {
                addedg(i * n + j + 1, t, w);
            }
        }
    }
    n = m * n + 2;
    while (augment())
        ;
    cout << sum - cut << endl;
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