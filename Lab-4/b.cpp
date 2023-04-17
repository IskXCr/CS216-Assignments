#include <iostream>
#include <queue>
#include <vector>
#include <set>

using namespace std;
using ll = long long;

struct Node
{
    ll c;
    int count;
    int parent;
    bool used;

    set<int> sons;

    Node() : c(0), count(1), parent(0), used(false) {}
};

struct Entry
{
    int id;
    double avg;

    Entry(int id, double avg) : id(id), avg(avg) {}

    friend bool operator<(const Entry &lhs, const Entry &rhs) { return lhs.avg < rhs.avg; }
};

int n, r;

vector<Node> nodes;

priority_queue<Entry> q;

int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);
    cin >> n >> r;
    r--;

    ll ans = 0;
    nodes.resize(n);
    for (int i = 0; i < n; ++i)
    {
        cin >> nodes[i].c;
        ans += nodes[i].c;
        if (i == r)
            continue;
        q.emplace(Entry(i, nodes[i].c));
    }

    for (int i = 0; i < n - 1; ++i)
    {
        int a, b;
        cin >> a >> b;
        a--, b--;
        nodes[b].parent = a;
        nodes[a].sons.emplace(b);
    }

    // Delete until nothing is in the queue
    while (!q.empty())
    {
        Entry e = q.top();
        q.pop();
        if (nodes[e.id].used)
            continue;
        nodes[e.id].used = true;
        // cout << "Selecting node: " << e.id << ", avg=" << e.avg << "\n";
        int father = nodes[e.id].parent;
        ans += (ll)nodes[e.id].c * nodes[father].count;
        for (auto i : nodes[e.id].sons)
            nodes[i].parent = father;
        nodes[father].sons.insert(nodes[e.id].sons.begin(), nodes[e.id].sons.end());
        nodes[father].c += nodes[e.id].c;
        nodes[father].count += nodes[e.id].count;
        if (father != r)
            q.push(Entry(father, (double)nodes[father].c / nodes[father].count));
    }
    cout << ans << "\n";
    return 0;
}