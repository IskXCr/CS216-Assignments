#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <cstring>
#include <stack>

using namespace std;

int board[5][5];

int ref_board[5][5] =
    {
        {1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1},
        {0, 0, 2, 1, 1},
        {0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0}};

static inline int convert(char c)
{
    if (c == '1')
        return 1;
    else if (c == '0')
        return 0;
    else
        return 2;
}

int eval()
{
    int res = 0;
    for (int i = 0; i < 5; ++i)
    {

        for (int j = 0; j < 5; ++j)
        {
            if (i == 2 && j == 2)
                continue;
            if (board[i][j] != ref_board[i][j])
                ++res;
        }
    }
    return res;
}

int safe(int x, int y)
{
    return !(x >= 5 || y >= 5 || x < 0 || y < 0);
}

int action[8][2] = {{1, 2}, {1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {-1, 2}, {-1, -2}};

int ans;

// depth starting from 1. Returns immediately when 1 result has been encountered.
// uses Astar to prune
void dfs(int x, int y, int depth, int up)
{
    for (int i = 0; i < 8; ++i)
    {
        int xd = x + action[i][0], yd = y + action[i][1];
        if (safe(xd, yd))
        {
            std::swap(board[xd][yd], board[x][y]);
            int rc = eval();
            if (rc == 0)
                ans = depth;
            else if (rc + depth <= up)
                dfs(xd, yd, depth + 1, up);
            std::swap(board[xd][yd], board[x][y]);
            if (ans != -1)
                return;
        }
    }
}

int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--)
    {
        int x0, y0;
        for (int i = 0; i < 5; ++i)
        {
            string s;
            cin >> s;
            for (int j = 0; j < 5; ++j)
            {
                board[i][j] = convert(s[j]);
                if (s[j] == '*')
                {
                    x0 = i;
                    y0 = j;
                }
            }
        }
        int rc = eval();
        if (rc == 0)
        {
            cout << "0\n";
        }
        else
        {
            ans = -1;
            for (int i = 1; i <= 15; ++i)
            {
                dfs(x0, y0, 1, i);
                if (ans != -1)
                    break;
            }
            cout << ans << "\n";
        }
    }
    return 0;
}