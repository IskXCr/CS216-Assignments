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

class Node
{
public:
    ll v;
    ll a; // additions required on this node
    ll b; // subtractions required on this node
    vector<int> neighbours;
    Node() : a{0}, b{0} {}
} nodes[100000 + 1];

int n;

void dfs(int t, int p)
{
    bool flag = false;
    ll mxa = 0, mxb = 0;
    for (int k: nodes[t].neighbours)
    {
        if (k != p)
        {
            dfs(k, t);
            if (!flag)
            {
                flag = true;
                mxa = nodes[k].a;
                mxb = nodes[k].b;
            }
            else
            {
                mxa = max(mxa, nodes[k].a);
                mxb = max(mxb, nodes[k].b);
            }
        }
    }
    // now there must be a mxa and a mxb
    ll rt = nodes[t].v + mxa - mxb;
    if (rt < 0)
    {
        mxa -= rt;
    }
    else if (rt > 0)
    {
        mxb += rt;
    }
    nodes[t].a = mxa;
    nodes[t].b = mxb;
}

void solve()
{
    cin >> n;
    if (n == 0)
    {
        cout << "0\n";
        return;
    }

    for (int i = 0; i < n - 1; ++i)
    {
        int a, b;
        cin >> a >> b;
        nodes[a].neighbours.push_back(b);
        nodes[b].neighbours.push_back(a);
    }
    for (int i = 1; i<= n; ++i)
        cin >> nodes[i].v;
    // root is 1.
    dfs(1, 0);
    cout << nodes[1].a + nodes[1].b << "\n";

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