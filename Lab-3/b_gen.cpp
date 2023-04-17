#include <iostream>
#include <vector>
#include <random>
#include <limits>
using namespace std;

int main()
{
    int n = 500;
    random_device rd;
    default_random_engine rng(rd());
    uniform_int_distribution<int> rand3(1, numeric_limits<int>().max());
    const int N = 1000000;
    cout << N << "\n";
    for (int i = 0; i < N; ++i)
        cout << rand3(rng) << " ";
    cout << "\n";
}