#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct Heap
{
    vector<int> container;

    Heap() : container(1) {}

    int front()
    {
        return container[1];
    }

    void insert(int *eval, int val)
    {
        container.push_back(val);
        int idx = container.size() - 1;
        while (idx > 1)
        {
            if (eval[container[idx]] < eval[container[idx / 2]])
            {
                swap(container[idx], container[idx / 2]);
                idx /= 2;
            }
            else
            {
                break;
            }
        }
    }

    void remove(int *eval)
    {
        int last = container.size() - 1;
        container[1] = container[last];
        container.resize(last);
        last--;
        int idx = 1;
        while (idx * 2 <= last)
        {
            if (idx * 2 + 1 <= last) /* Compare two */
            {
                if (eval[container[idx]] < eval[container[idx * 2]] && eval[container[idx]] < eval[container[idx * 2 + 1]])
                    break;
                int min = eval[container[idx * 2]] < eval[container[idx * 2 + 1]] ? idx * 2 : idx * 2 + 1;
                swap(container[idx], container[min]);
                idx = min;
            }
            else /* Compare 1 */
            {
                if (eval[container[idx]] < eval[container[idx * 2]])
                    break;
                swap(container[idx], container[idx * 2]);
                idx *= 2;
            }
        }
    }

    size_t size()
    {
        return container.size() - 1;
    }
};

int col_eval[1001][1001];
int col_cap[1001];
Heap col_queue[1001];
vector<int> stu_rank[1001]; /* Store College ID in ranking order */
int stu_cur[1001];
int stu_pos[1001]; // Debug

int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    int n, m;
    cin >> n >> m;

    for (int i = 1; i <= m; ++i)
        cin >> col_cap[i];

    for (int i = 1; i <= n; ++i)
    {
        vector<pair<int, int>> temp;
        for (int j = 1; j <= m; ++j)
        {
            int s;
            cin >> s;
            if (s > 0)
                temp.push_back(make_pair(j, s));
        }
        sort(temp.begin(), temp.end(), [](const pair<int, int> &a, pair<int, int> &b)
             { return a.second > b.second; });
        for (auto entry : temp)
        {
            stu_rank[i].push_back(entry.first);
        }
    }

    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j)
            cin >> col_eval[i][j];

    queue<int> q;
    for (int i = 1; i <= n; ++i)
    {
        if (stu_rank[i].size() > 0)
            q.push(i);
    }

    while (!q.empty())
    {
        /**
         * Algorithm:
         *     Select the current student.
         *     Place the student into the college if his value is positive. (Compare with the lowest one's eval.)
         *         If cannot place, place into the queue if still has remaining to select.
         *         If can place, just place, and send the one kicked out to the queue.
         */
        int stu = q.front();
        int col = stu_rank[stu][stu_cur[stu]++];
        q.pop();
        // cerr << "Examining " << stu << endl;
        if (col_eval[col][stu] > 0)
        {
            if (col_queue[col].size() < col_cap[col])
            {
                col_queue[col].insert(col_eval[col], stu);
                // cerr << "Inserted " << stu << " to " << col << endl;
                stu_pos[stu] = col;
            }
            else if (col_eval[col][col_queue[col].front()] < col_eval[col][stu]) /* Smaller front */
            {
                int stu1 = col_queue[col].front();
                col_queue[col].remove(col_eval[col]);
                col_queue[col].insert(col_eval[col], stu);
                if (stu_cur[stu1] < stu_rank[stu1].size())
                {
                    stu_pos[stu1] = 0;
                    q.push(stu1);
                }
                stu_pos[stu] = col;
                // cerr << "College value rejected for new insertion. Pushing " << stu1 << endl;
                // cerr << "Inserted " << stu << " to " << col << endl;
            }
            else if (stu_cur[stu] < stu_rank[stu].size()) /* Rejected but still in queue*/
            {
                // cerr << "College full, rejected. Pushing " << stu << endl;
                q.push(stu);
            }
        }
        else if (stu_cur[stu] < stu_rank[stu].size())
        {
            // cerr << "College value rejected. Pushing " << stu << endl;
            q.push(stu);
        }
    }

    // cout << "Reached\n";

    for (int i = 1; i <= m; ++i)
    {
        size_t sz = col_queue[i].size();
        cout << sz << " ";
        for (int j = 1; j <= sz; ++j)
        {
            cout << col_queue[i].container[j];
            if (j < sz)
                cout << " ";
        }
        cout << "\n";
    }

    return 0;
}