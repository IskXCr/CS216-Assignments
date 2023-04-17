#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// using namespace std;
using ll = long long;
int main()
{
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<ll> a(n);
    for (int i = 0; i < n; ++i)
        std::cin >> a[i];
    ll avg = std::accumulate(a.begin(), a.end(), 0LL) / n;
    std::vector<ll> c(n);
    c[0] = a[0] - avg;
    for (int i = 1; i < n; ++i)
        c[i] = c[i - 1] + a[i] - avg;
    std::sort(c.begin(), c.end());
    ll x1 = c[n / 2];
    ll ans = 0;
    for (int i = 0; i < n; ++i)
        ans += std::abs(x1 - c[i]);
    std::cout << ans << "\n";
    return 0;
}