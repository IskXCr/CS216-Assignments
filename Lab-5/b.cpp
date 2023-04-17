#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <chrono>
#include <cstring>

using ll = long long;

struct Edge
{
    Edge() : src{-1}, dst{-1}, w{0}, mw{0} {}
    Edge(int src, int dst, ll w) : src{src}, dst{dst}, w{w}, mw{w} {}
    Edge(int src, int dst, ll w, ll mw) : src{src}, dst{dst}, w{w}, mw{mw} {}
    friend bool operator<(const Edge &l, const Edge &r) { return l.mw < r.mw; }
    int src;
    int dst;
    ll w;
    ll mw; /* modified weight */
};
/* Leftist heap */
namespace hos
{
    struct hn;
    struct hp;
    hn *mgi(hn *l, hn *r);
    hn *mh(hn *l, hn *r);
    void cg(hn *n, ll wf);
    void pd(hn *n);
    hp mg(hp &l, hp &r);
    void r(hp &h);
    void r(hn *n);
}

struct hos::hn
{
    inline void i(Edge &element, hn *left, hn *right, int dist, ll tag)
    {
        this->element = element;
        this->left = left;
        this->right = right;
        this->dist = dist;
        this->tag = tag;
    }
    hn(Edge &element)
    {
        i(element, nullptr, nullptr, 0, 0LL);
    }
    Edge element;
    hn *left, *right;
    int dist;
    ll tag;
};

struct hos::hp
{
    hp() : rt{nullptr} {}
    hp(hn *rt) : rt{rt} {}
    Edge em()
    {
        pd(rt);
        Edge ret = rt->element;
        hn *left = rt->left;
        hn *right = rt->right;
        delete rt;
        rt = mh(left, right);
        return ret;
    }
    void is(Edge &edge)
    {
        rt = mh(rt, new hn(edge));
        return;
    }
    void is(Edge &&edge)
    {
        rt = mh(rt, new hn(edge));
        return;
    }
    void m(ll woffset)
    {
        cg(rt, woffset);
    }
    bool e()
    {
        return rt == nullptr;
    }
    hn *rt;
};
/* accepts nullptr */
void hos::cg(hn *node, ll woffset)
{
    if (node == nullptr)
        return;
    node->element.mw += woffset;
    node->tag += woffset;
    return;
}
void hos::pd(hn *node)
{
    if (node == nullptr || node->tag == 0LL)
        return;
    cg(node->left, node->tag);
    cg(node->right, node->tag);
    node->tag = 0LL;
    return;
}
hos::hn *hos::mgi(hn *lhs, hn *rhs)
{
    /* lhs always has a smaller element */
    if (lhs->left == nullptr)
        lhs->left = rhs;
    else
    {
        lhs->right = mh(lhs->right, rhs);
        if (lhs->left->dist < lhs->right->dist)
            std::swap(lhs->left, lhs->right);
        lhs->dist = lhs->right->dist + 1;
    }
    return lhs;
}
hos::hn *hos::mh(hn *lhs, hn *rhs)
{
    if (lhs == nullptr)
        return rhs;
    if (rhs == nullptr)
        return lhs;
    pd(lhs);
    pd(rhs);
    if (lhs->element < rhs->element)
        return mgi(lhs, rhs);
    else
        return mgi(rhs, lhs);
}
hos::hp hos::mg(hp &lhs, hp &rhs)
{
    hp heap;
    heap.rt = mh(lhs.rt, rhs.rt);
    lhs.rt = rhs.rt = nullptr;
    return heap;
}
void hos::r(hn *node)
{
    if (node->left != nullptr)
        r(node->left);
    if (node->right != nullptr)
        r(node->right);
    delete node;
    return;
}
void hos::r(hp &heap)
{
    if (heap.rt != nullptr)
    {
        r(heap.rt);
        heap.rt = nullptr;
    }
    return;
}

const int maxv = 3000;
Edge sample;
int total;
Edge in[maxv];
int prev[maxv];
int parent[maxv];
int root[maxv];
std::vector<int> children[maxv];

Edge bkup_in[maxv];
int bkup_prev[maxv];
int bkup_parent[maxv];
int bkup_root[maxv];
std::vector<int> bkup_children[maxv];
hos::hp queue[maxv];

int n, m;

int find(int v)
{
    std::vector<int> list;
    // list.reserve(n);
    while (root[v] != -1)
    {
        // std::cout << "finding node " << vertices[v] << "\n";
        list.push_back(v);
        v = root[v];
    }
    for (auto idx : list)
        root[idx] = v;
    return v;
}

void contract(int a)
{
    while (!queue[a].e())
    {
        Edge edge = queue[a].em();
        int b = find(edge.src);
        if (a != b)
        {
            in[a] = edge;
            prev[a] = b;
            if (in[edge.src].src == -1)
            {
                a = b;
            }
            else // do contraction
            {
                int c = total++;

                for (; a != c; a = find(prev[a]))
                {
                    parent[a] = c;
                    root[a] = c;
                    queue[a].m(-in[a].mw);

                    children[c].push_back(a);

                    queue[c] = hos::mg(queue[c], queue[a]); // meld
                }
                a = c;
            }
        }
    }
}
int pending[maxv];
int begin, end;
void dismantle(int v)
{
    while (parent[v] != -1)
    {
        v = parent[v];
        int u;
        for (int i = 0, j = children[v].size(); i < j; ++i)
        {
            parent[u = children[v][i]] = -1;
            if (children[u].size())
                pending[++end] = u;
        }
        children[v].clear();
    }
}

void expand(int root)
{
    begin = end = 0;
    dismantle(root);

    while (begin != end)
    {
        int c = pending[++begin];
        Edge edge = in[c];
        int v = edge.dst;
        in[v] = edge;
        dismantle(v);
    }
}

void reset_all()
{
    for (int i = 0; i < maxv; ++i)
    {
        children[i].clear();
        hos::r(queue[i]);
    }

    std::fill(in, in + maxv, sample);
    std::fill(prev, prev + maxv, -1);
    std::fill(parent, parent + maxv, -1);
    std::fill(root, root + maxv, -1);
    total = 0;
}
void save_all()
{
    for (int i = 0; i < total; ++i)
    {
        int k = children[i].size();
        bkup_children[i].clear();
        for (int j = 0; j < k; ++j)
            bkup_children[i].push_back(children[i][j]);
        // std::cout << "saving " << k << " children for " << i << "\n";
    }
    memcpy(bkup_in, in, total * sizeof(Edge));
    memcpy(bkup_prev, prev, total * sizeof(int));
    memcpy(bkup_parent, parent, total * sizeof(int));
}

void load_all()
{
    for (int i = 0; i < total; ++i)
    {
        children[i].clear();
        int k = bkup_children[i].size();
        for (int j = 0; j < k; ++j)
            children[i].push_back(bkup_children[i][j]);
    }
    memcpy(in, bkup_in, total * sizeof(Edge));
    memcpy(prev, bkup_prev, total * sizeof(int));
    memcpy(parent, bkup_parent, total * sizeof(int));
}

int main()
{
    using std::cin;
    using std::cout;
    using namespace std::chrono;
    cin.tie(nullptr);
    cin.sync_with_stdio(false);
    ll vweight = 0x7fffffffLL;
    cin >> n >> m;
    reset_all();
    total = n + 1;
    int u, v;
    ll w;
    for (int i = 0; i < m; ++i)
    {
        cin >> u >> v >> w;
        queue[v + 1].is(Edge(u + 1, v + 1, w));
    }

    for (int i = 1; i <= n; ++i) { queue[i].is(Edge(i % n + 1, i, vweight)); }

    ll ans = vweight;
    int pos = -1;
    contract(1);
    save_all();
    for (int r = 1; r <= n; ++r)
    {
        load_all();
        expand(r);
        ll tmp = 0;
        bool flag = false;
        for (int i = 1; i <= n; ++i)
        {
            if (i != r)
            {
                if (in[i].w == vweight) { flag = true; break; }
                tmp += in[i].w;
            }
        }
        if (!flag && tmp < ans) { ans = tmp; pos = r;}
    }
    if (ans >= vweight)
        cout << "impossible\n\n";
    else
        cout << ans << " " << pos - 1 << "\n\n";
    return 0;
}