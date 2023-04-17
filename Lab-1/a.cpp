#include <iostream>
#include <queue>
#include <string>
#include <map>

using namespace std;

using ll = long long;

int pref_list[1000][1000];
int pref_cursor[1000]; // next available position
int pref_list2[1000][1000];
int g_match[1000];

map<string, int> gmap_name;
map<int, string> gmap_no;
map<string, int> bmap_name;
map<int, string> bmap_no;

int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);
    int n;
    cin >> n;

    for (int i = 0; i < n; ++i)
    {
        string s;
        cin >> s;
        bmap_name[s] = i;
        bmap_no[i] = s;
    }

    for (int i = 0; i < n; ++i)
    {
        string s;
        cin >> s;
        gmap_name[s] = i;
        gmap_no[i] = s;
    }

    for (int i = 0; i < n; ++i)
    {
        string s;
        for (int j = 0; j < n; ++j)
        {
            cin >> s;
            pref_list[i][j] = gmap_name[s];
        }
    }

    for (int i = 0; i < n; ++i)
    {
        string s;
        for (int j = 0; j < n; ++j)
        {
            cin >> s;
            pref_list2[i][bmap_name[s]] = j;
        }
    }

    queue<int> q;
    for (int i = 0; i < n; ++i)
    {
        q.push(i);
        g_match[i] = -1;
    }
    while (!q.empty())
    {
        int i = q.front();
        q.pop();
        int target = pref_list[i][pref_cursor[i]];
        if (g_match[target] == -1) // If no one
        {
            g_match[target] = i;
        }
        else if (pref_list2[target][g_match[target]] > pref_list2[target][i]) // If i is more attractive
        {
            q.push(g_match[target]);
            g_match[target] = i;
        }
        else // i is not attractive
        {
            q.push(i);
        }
        pref_cursor[i]++;
    }

    // output
    for (int i = 0; i < n; ++i)
    {
        // Debug: off
        cout << bmap_no[g_match[i]] << " " << gmap_no[i] << "\n";
    }
    return 0;
}