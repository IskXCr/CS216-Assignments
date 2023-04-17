#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<pair<int, int>> arr(n);
    vector<int> slots(n);
    vector<int> prefetch(n); // next available position
    for (int i = 0; i < n; ++i)
        prefetch[i] = i;
    for (int i = 0; i < n; ++i)
        cin >> arr[i].second;
    for (int i = 0; i < n; ++i)
        cin >> arr[i].first;
    sort(arr.begin(), arr.end(), [](const pair<int, int> &lhs, const pair<int, int> &rhs)
         { return lhs.first > rhs.first || (lhs.first == rhs.first && lhs.second > rhs.second); });
    int ans = 0;
    for (auto entry : arr)
    {
        int t = prefetch[entry.second - 1];
        while (t >= 0 && slots[t] != 0)
        {
            t = prefetch[t];
            // cout << "t = " << t << ", prefetch[t] = " << prefetch[t] << endl;
        }
        if (t >= 0)
        {
            slots[t] = 1;
            prefetch[t] = t - 1;
        }
        else
        {
            ans += entry.first;
        }
        if (t != entry.second - 1)
            prefetch[entry.second - 1] = t - 1;
    }
    cout << ans << endl;
    return 0;
}