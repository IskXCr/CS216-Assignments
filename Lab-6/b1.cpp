#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <limits>
#include <cmath>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace std::chrono;
using ll = long long;
using Point = pair<int, int>;

ll fuck(vector<Point> points)
{
    if (points.size() == 1)
    {
        return numeric_limits<ll>().max();
    }
    if (points.size() == 2)
    {
        ll dx = points[0].first - points[1].first;
        ll dy = points[0].second - points[1].second;
        return dx * dx + dy * dy;
    }
    vector<Point> fuck1, fuck2;
    size_t half = points.size() / 2;
    ll L = points[half].first;
    fuck1.insert(fuck1.end(), points.begin(), points.begin() + half);
    fuck2.insert(fuck2.end(), points.begin() + half, points.end());
    ll delta1 = fuck(fuck1);
    ll delta2 = fuck(fuck2);
    ll delta = min(delta1, delta2);
    ll _delta = (ll)(ceil(sqrt(delta)));
    auto fuck3 = remove_if(points.begin(), points.end(), [L, _delta](const Point &point)
                           { return point.first < L - _delta || point.first > L + _delta; });
    points.resize(fuck3 - points.begin());
    sort(points.begin(), points.end(), [](const Point &lhs, const Point &rhs)
         { return lhs.second < rhs.second; });
    for (size_t i = 0; i < points.size(); ++i)
    {
        for (size_t j = i + 1; j < min((size_t)(i + 7), points.size()); ++j)
        {
            ll dx = points[i].first - points[j].first;
            ll dy = points[i].second - points[j].second;
            ll r = dx * dx + dy * dy;
            if (r < delta)
                delta = r;
        }
    }
    return delta;
}

int main()
{
    auto t1 = high_resolution_clock::now();
    cin.tie(nullptr);
    cin.sync_with_stdio(false);
    int n, x, y;
    cin >> n;
    vector<Point> arr;
    for (int i = 0; i < n; ++i)
    {
        cin >> x >> y;
        arr.push_back(make_pair(x, y));
    }
    sort(arr.begin(), arr.end(), [](pair<int, int> &lhs, pair<int, int> &rhs)
         { return lhs.first < rhs.first; });
    cout << fuck(arr) << endl;
    // cout << fixed << setprecision(4) << sqrt((long double)fuck(arr)) << endl;
    auto t2 = high_resolution_clock::now();
    std::cout << "[T] Took " << duration_cast<duration<double>>(t2 - t1).count() << " s\n";

    return 0;
}