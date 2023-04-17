#include <iostream>
#include <map>
#include <random>
#include <vector>

using namespace std;

int main()
{
    const int N = 1000;
    random_device rd;
    default_random_engine rng(rd());
    uniform_int_distribution<int> rand3(0, N - 1);

    cout << N << endl;
    for (int i = 0; i < N; ++i)
        cout << i << " ";
    cout << endl;
    for (int i = 0; i < N; ++i)
        cout << i << " ";
    cout << endl;

    for (int u = 0; u < 2; ++u)
    {
        for (int i = 0; i < N; ++i)
        {
            vector<int> gen(N);
            for (int j = 0; j < N; ++j)
            {
                int k;
                do
                {
                    k = rand3(rng);
                } while (gen[k] == 1);
                gen[k] = 1;
                cout << k << " ";
            }
            cout << "\n";
        }
    }
}