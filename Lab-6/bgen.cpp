#include <bits/stdc++.h>

using namespace std;

int main()
{
    constexpr int n = 100000;
    constexpr int mod = 20000000;
    constexpr int hmod = 10000000;
    cout << n << "\n";
    srand(time(nullptr));
    for (int i = 0; i < n; ++i)
    {
        cout << (rand() % mod - hmod) << " " << (rand() % mod - hmod) << "\n";
    }
    return 0;
}