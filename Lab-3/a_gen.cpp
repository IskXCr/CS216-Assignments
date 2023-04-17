#include <iostream>
#include <random>
using namespace std;

int main()
{
    int n = 500;
    random_device rd;
    default_random_engine rng(rd());
    uniform_int_distribution<int> rand3(1, n - 1);
    cout << n << "\n";
    for (int i = 0; i < n; ++i)
        cout << rand3(rng) << " ";
    cout << "\n";
    for (int i = 0; i < n; ++i)
        cout << rand3(rng) << " ";
    cout << "\n";
    return 0;
}