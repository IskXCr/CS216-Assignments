#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <random>
#include <set>

using namespace std;

int main()
{
    random_device rd;
    default_random_engine rng(rd());
    uniform_int_distribution<int> rand3(1, 1000);
    uniform_int_distribution<int> rand4(-1000, 1000);

    // int N = rand3(rng);
    // int M = rand3(rng);
    int N = 1000;
    int M = 1000;

    cout << N << " " << M << "\n";

    uniform_int_distribution<int> randCap(1, N);
    for (int i = 0; i < M; ++i)
        cout << randCap(rng) << " ";
    cout << "\n";

    for (int i = 0; i < N; ++i)
    {
        int q;
        set<int> vals;
        vals.emplace(0);
        for (int j = 0; j < M; ++j)
        {
            while (vals.count(q = rand4(rng)) > 0)
                ;
            cout << q << " ";
        }
        cout << "\n";
    }

    for (int i = 0; i < M; ++i)
    {
        int q;
        set<int> vals;
        vals.emplace(0);
        for (int j = 0; j < N; ++j)
        {
            while (vals.count(q = rand4(rng)) > 0)
                ;
            cout << q << " ";
        }
        cout << "\n";
    }
}