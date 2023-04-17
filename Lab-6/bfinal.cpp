#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <limits>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <cstring>

using namespace std;
using namespace std::chrono;
using ll = long long;
constexpr int maxn = 400000;
int x[maxn], y[maxn], xb[maxn], yb[maxn];
int n;

ll solve(int l, int r)
{
    int len = r - l;
    if (len <= 1)
    {
        return numeric_limits<ll>().max();
    }
    if (len == 2)
    {
        int _ = l + 1;
        ll dx = x[_] - x[l];
        ll dy = y[_] - y[l];
        if (y[l] > y[_])
        {
            std::swap(x[_], x[l]);
            std::swap(y[_], y[l]);
        }
        return dx * dx + dy * dy;
    }
    int half = len / 2;
    int smid = l + half;
    ll L = x[smid];

    ll delta1, delta2;
    delta1 = solve(l, smid);
    delta2 = solve(smid, r);

    int lc = l, rc = smid, mt = l;
    while (mt < r)
    {
        if (rc >= r || (lc < smid && y[lc] < y[rc]))
            xb[mt] = x[lc], yb[mt] = y[lc], lc++;
        else
            xb[mt] = x[rc], yb[mt] = y[rc], rc++;
        mt++;
    }
    memcpy(x + l, xb + l, len * sizeof(int));
    memcpy(y + l, yb + l, len * sizeof(int));

    ll delta = min(delta1, delta2);
    ll _delta = (ll)(ceil(sqrt(delta)));

    int total = 0;
    for (int i = l; i < r; ++i)
        if (x[i] >= L - _delta && x[i] <= L + _delta)
            xb[total] = x[i], yb[total] = y[i], total++;

    for (size_t i = 0; i < total; ++i)
    {
        size_t ubd = min(i + 8, (size_t)total);
        for (size_t j = i + 1; j < ubd; ++j)
        {
            // if (yb[j] - yb[i] > _delta)
            //     break;
            ll dx = xb[i] - xb[j];
            ll dy = yb[i] - yb[j];
            ll r = dx * dx + dy * dy;
            if (r < delta)
                delta = r;
        }
    }
    // if (delta == 0)
    //     cout << "\trecur with size " << len << ", result=" << delta << "\n";
    return delta;
}

int main()
{
    auto t1 = high_resolution_clock::now();
    cin.tie(nullptr);
    cin.sync_with_stdio(false);
    int x0, y0;
    cin >> n;
    int *xarr = new int[n];
    for (int i = 0; i < n; ++i)
    {
        xarr[i] = i;
    }
    for (int i = 0; i < n; ++i)
    {
        cin >> x[i] >> y[i];
    }
    sort(xarr, xarr + n, [](int lhs, int rhs)
         { return x[lhs] < x[rhs]; });
    for (int i = 0; i < n; ++i)
    {
        xb[i] = x[xarr[i]];
        yb[i] = y[xarr[i]];
    }
    memcpy(x, xb, n * sizeof(int));
    memcpy(y, yb, n * sizeof(int));

    cout << solve(0, n) << endl;

    auto t2 = high_resolution_clock::now();
    std::cout << "[T] Took " << duration_cast<duration<double>>(t2 - t1).count() << " s\n";

    return 0;
}