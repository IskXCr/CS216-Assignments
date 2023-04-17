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
int x[maxn], y[maxn], flag[maxn], xb[maxn], yb[maxn];
vector<int> xarr;
vector<int> yarr;
int n;

ll solve(int l, int r, vector<int> &yarr)
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
        return dx * dx + dy * dy;
    }
    int half = len / 2;
    int smid = l + half;
    ll L = x[smid];
    // std::fill(flag + l, flag + smid, 1);
    // std::fill(flag + smid, flag + len, 0);
    vector<int> yarrl, yarrr;
    yarrl.reserve(half + 1);
    yarrr.reserve(half + 1);
    for (int i = 0; i < yarr.size(); ++i)
        if (yarr[i] < smid)
            yarrl.push_back(yarr[i]);
        else
            yarrr.push_back(yarr[i]);

    ll delta1, delta2;
    delta1 = solve(l, smid, yarrl);
    delta2 = solve(smid, r, yarrr);

    ll delta = min(delta1, delta2);
    ll _delta = (ll)(ceil(sqrt(delta)));

    vector<int> candidate_list;
    candidate_list.reserve(len);
    for (int i = 0; i < yarr.size(); ++i)
        if (x[yarr[i]] >= L - _delta && x[yarr[i]] <= L + _delta)
            candidate_list.push_back(yarr[i]);

    for (size_t i = 0; i < candidate_list.size(); ++i)
    {
        size_t ubd = min((size_t)(i + 7), candidate_list.size());
        for (size_t j = i + 1; j < ubd; ++j)
        {
            ll dx = x[candidate_list[i]] - x[candidate_list[j]];
            ll dy = y[candidate_list[i]] - y[candidate_list[j]];
            ll r = dx * dx + dy * dy;
            if (r < delta)
                delta = r;
        }
    }
    // cout << "recur with size " << xarr.size() << ", result=" << delta << "\n";
    return delta;
}

int main()
{
    // auto t1 = high_resolution_clock::now();
    cin.tie(nullptr);
    cin.sync_with_stdio(false);
    int x0, y0;
    cin >> n;
    for (int i = 0; i < n; ++i)
    {
        xarr.push_back(i);
        yarr.push_back(i);
    }
    for (int i = 0; i < n; ++i)
    {
        cin >> x[i] >> y[i];
    }
    sort(xarr.begin(), xarr.end(), [](int lhs, int rhs)
         { return x[lhs] < x[rhs]; });
    for (int i = 0; i < n; ++i)
    {
        xb[i] = x[xarr[i]];
        yb[i] = y[xarr[i]];
    }
    memcpy(x, xb, n * sizeof(int));
    memcpy(y, yb, n * sizeof(int));
    sort(yarr.begin(), yarr.end(), [](int lhs, int rhs)
         { return y[lhs] < y[rhs]; });

    cout << solve(0, n, yarr) << endl;
    // cout << fixed << setprecision(4) << sqrt((long double)fuck(arr)) << endl;

    // auto t2 = high_resolution_clock::now();
    // std::cout << "[T] Took " << duration_cast<duration<double>>(t2 - t1).count() << " s\n";

    return 0;
}