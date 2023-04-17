#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <limits>
#include <cmath>
#include <iomanip>
#include <ranges>
#include <chrono>

using namespace std;
using namespace std::chrono;
using ll = long long;
struct Point
{
    int x;
    int y;
    int flag;
};

ll fuck(vector<Point *> &xarr, vector<Point *> &yarr)
{
    if (xarr.size() <= 1)
    {
        return numeric_limits<ll>().max();
    }
    if (xarr.size() == 2)
    {
        ll dx = xarr[0]->x - xarr[1]->x;
        ll dy = xarr[0]->y - xarr[1]->y;
        return dx * dx + dy * dy;
    }
    size_t half = xarr.size() / 2;
    ll L = xarr[half]->x;
    for (size_t i = 0; i < half; ++i)
        xarr[i]->flag = 1;
    for (size_t i = half; i < xarr.size(); ++i)
        xarr[i]->flag = 0;
    vector<Point *> fuck1, fuck2, fuck3, fuck4;
    for (auto i : xarr)
        if (i->flag)
            fuck1.push_back(i);
        else
            fuck3.push_back(i);
    for (auto i : yarr)
        if (i->flag)
            fuck2.push_back(i);
        else
            fuck4.push_back(i);

    ll delta1, delta2;
    delta1 = fuck(fuck1, fuck2);
    delta2 = fuck(fuck3, fuck4);

    ll delta = min(delta1, delta2);
    ll _delta = (ll)(ceil(sqrt(delta)));

    vector<Point *> fuck5;
    for (auto point : yarr)
        if ((point->x >= L - _delta) && (point->x <= L + _delta))
            fuck5.push_back(point);

    for (size_t i = 0; i < fuck5.size(); ++i)
    {
        for (size_t j = i + 1; j < min((size_t)(i + 7), fuck5.size()); ++j)
        {
            ll dx = fuck5[i]->x - fuck5[j]->x;
            ll dy = fuck5[i]->y - fuck5[j]->y;
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
    auto t1 = high_resolution_clock::now();
    cin.tie(nullptr);
    cin.sync_with_stdio(false);
    int n, x, y;
    cin >> n;
    vector<Point *> xarr;
    vector<Point *> yarr;
    Point *fucked = new Point[n];
    for (int i = 0; i < n; ++i)
    {
        cin >> x >> y;
        fucked[i].x = x;
        fucked[i].y = y;
        xarr.push_back(fucked + i);
        yarr.push_back(fucked + i);
    }
    sort(xarr.begin(), xarr.end(), [](Point *lhs, Point *rhs)
         { return lhs->x < rhs->x; });
    sort(yarr.begin(), yarr.end(), [](Point *lhs, Point *rhs)
         { return lhs->y < rhs->y; });

    cout << fuck(xarr, yarr) << endl;
    // cout << fixed << setprecision(4) << sqrt((long double)fuck(arr)) << endl;

    auto t2 = high_resolution_clock::now();
    std::cout << "[T] Took " << duration_cast<duration<double>>(t2 - t1).count() << " s\n";

    return 0;
}