#include <iostream>

using namespace std;

int main()
{
    int n, k;
    cin >> n >> k;
    if (k > (n + 1) / 2)
        k = n - k + 1;
    cout << (n == 1 ? 1 : k * 2) << "\n";
    return 0;
}