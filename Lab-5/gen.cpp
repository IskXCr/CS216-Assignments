#include <bits/stdc++.h>

using namespace std;

int main()
{
    int n = 1000;
    int m = 10000;
    int w = 1000;
    cout << n << " " << m << endl;
    srand(time(nullptr));
    for (int i = 0; i < m; ++i)
    {
        cout << (rand() % n) << " " << (rand() % n) << " " << (rand() % w + 1) << "\n";
    }
    return 0;
}