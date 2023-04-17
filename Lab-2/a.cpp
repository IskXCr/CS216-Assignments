#include <iostream>

using namespace std;

int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);
    int t, n;
    cin >> t;
    while (t-- > 0)
    {
        cin >> n;
        cout << (n % 6 != 0 ? "Alice" : "Bob") << "\n";
    }
    return 0;
}