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
    Edge() : src{-1}, dst{-1}, weight{0}, m_weight{0} {}

    Edge(int src, int dst, ll weight) : src{src}, dst{dst}, weight{weight}, m_weight{weight} {}

    Edge(int src, int dst, ll weight, ll m_weight) : src{src}, dst{dst}, weight{weight}, m_weight{m_weight} {}

    friend bool operator<(const Edge &lhs, const Edge &rhs) { return lhs.m_weight < rhs.m_weight; }

    // friend bool operator==(const Edge &lhs, const Edge &rhs) { return lhs.m_weight == rhs.m_weight && lhs.src == rhs.src && lhs.dst == rhs.dst; }

    int src;
    int dst;
    ll weight;
    ll m_weight; /* modified weight */
};

std::ostream &operator<<(std::ostream &os, const Edge &e)
{
    return os << "edge (" << e.src << "->" << e.dst << ") with weight=" << e.weight << ", m_weight=" << e.m_weight;
}

/* Leftist heap */
namespace Heap_Operations
{
    struct Heap_Node;

    struct Heap;

    Heap make_heap(std::vector<int> &vec);

    Heap_Node *merge_heap_impl1(Heap_Node *lhs, Heap_Node *rhs);

    Heap_Node *merge_heap(Heap_Node *lhs, Heap_Node *rhs);

    void change(Heap_Node *node, ll woffset);

    void pushdown(Heap_Node *node);

    Heap merge_heap(Heap &lhs, Heap &rhs);

    void release(Heap &heap);

    void release(Heap_Node *node);
}

struct Heap_Operations::Heap_Node
{

    inline void initialize(Edge &element, Heap_Node *left, Heap_Node *right, int dist, ll tag)
    {
        this->element = element;
        this->left = left;
        this->right = right;
        this->dist = dist;
        this->tag = tag;
    }

    Heap_Node(Edge &element)
    {
        initialize(element, nullptr, nullptr, 0, 0LL);
    }

    Edge element;
    Heap_Node *left, *right;
    int dist;
    ll tag;
};

struct Heap_Operations::Heap
{
    Heap() : rt{nullptr} {}

    Heap(Heap_Node *rt) : rt{rt} {}

    Edge extract_min()
    {
        pushdown(rt);
        Edge ret = rt->element;
        Heap_Node *left = rt->left;
        Heap_Node *right = rt->right;
        delete rt;
        rt = merge_heap(left, right);
        return ret;
    }

    void insert(Edge &edge)
    {
        rt = merge_heap(rt, new Heap_Node(edge));
        return;
    }

    void insert(Edge &&edge)
    {
        rt = merge_heap(rt, new Heap_Node(edge));
        return;
    }

    void modify(ll woffset)
    {
        change(rt, woffset);
    }

    bool empty()
    {
        return rt == nullptr;
    }

    Heap_Node *rt;
};

/* accepts nullptr */
void Heap_Operations::change(Heap_Node *node, ll woffset)
{
    if (node == nullptr)
        return;
    node->element.m_weight += woffset;
    node->tag += woffset;
    return;
}

void Heap_Operations::pushdown(Heap_Node *node)
{
    if (node == nullptr || node->tag == 0LL)
        return;
    change(node->left, node->tag);
    change(node->right, node->tag);
    node->tag = 0LL;
    return;
}

Heap_Operations::Heap_Node *Heap_Operations::merge_heap_impl1(Heap_Node *lhs, Heap_Node *rhs)
{
    /* lhs always has a smaller element */
    if (lhs->left == nullptr)
        lhs->left = rhs;
    else
    {
        lhs->right = merge_heap(lhs->right, rhs);
        if (lhs->left->dist < lhs->right->dist)
            std::swap(lhs->left, lhs->right);
        lhs->dist = lhs->right->dist + 1;
    }
    return lhs;
}

Heap_Operations::Heap_Node *Heap_Operations::merge_heap(Heap_Node *lhs, Heap_Node *rhs)
{
    if (lhs == nullptr)
        return rhs;
    if (rhs == nullptr)
        return lhs;
    pushdown(lhs);
    pushdown(rhs);
    if (lhs->element < rhs->element)
        return merge_heap_impl1(lhs, rhs);
    else
        return merge_heap_impl1(rhs, lhs);
}

Heap_Operations::Heap Heap_Operations::merge_heap(Heap &lhs, Heap &rhs)
{
    Heap heap;
    heap.rt = merge_heap(lhs.rt, rhs.rt);
    lhs.rt = rhs.rt = nullptr;
    return heap;
}

void Heap_Operations::release(Heap_Node *node)
{
    if (node->left != nullptr)
        release(node->left);
    if (node->right != nullptr)
        release(node->right);
    delete node;
    return;
}

void Heap_Operations::release(Heap &heap)
{
    if (heap.rt != nullptr)
    {
        release(heap.rt);
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
Heap_Operations::Heap queue[maxv];

int n, m, r;

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
    while (!queue[a].empty())
    {
        Edge edge = queue[a].extract_min();

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
                    queue[a].modify(-in[a].m_weight);

                    children[c].push_back(a);

                    queue[c] = Heap_Operations::merge_heap(queue[c], queue[a]); // meld
                }
                a = c;
            }
        }
    }
}

int pending[maxv];
int begin, end;

// static decltype(std::chrono::high_resolution_clock::now() - std::chrono::high_resolution_clock::now()) accum0;

void dismantle(int v)
{
    // using namespace std::chrono;
    // auto t1 = high_resolution_clock::now();
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
    // auto t2 = high_resolution_clock::now();
    // accum0 += (t2 - t1);
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
        Heap_Operations::release(queue[i]);
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
    // memcpy(bkup_root, root, total * sizeof(int));
}
// static decltype(std::chrono::high_resolution_clock::now() - std::chrono::high_resolution_clock::now()) accum1;

void load_all()
{
    // using namespace std::chrono;
    // auto t1 = high_resolution_clock::now();
    for (int i = 0; i < total; ++i)
    {
        children[i].clear();
        int k = bkup_children[i].size();
        // std::cout << "loading " << k << " children for " << i << "\n";
        for (int j = 0; j < k; ++j)
            children[i].push_back(bkup_children[i][j]);
    }
    memcpy(in, bkup_in, total * sizeof(Edge));
    memcpy(prev, bkup_prev, total * sizeof(int));
    memcpy(parent, bkup_parent, total * sizeof(int));
    // memcpy(root, bkup_root, total * sizeof(int));

    // auto t2 = high_resolution_clock::now();
    // accum1 += (t2 - t1);
}

int main()
{
    using std::cin;
    using std::cout;
    using namespace std::chrono;

    cin.tie(nullptr);
    cin.sync_with_stdio(false);
    ll vweight = 0x7fffffffLL;

    cin >> n >> m >> r;
    reset_all();

    total = n + 1;

    int u, v;
    ll w;

    for (int i = 0; i < m; ++i)
    {
        cin >> u >> v >> w;
        queue[v].insert(Edge(u, v, w));
    }

    for (int i = 1; i <= n; ++i)
    {
        queue[i].insert(Edge(i % n + 1, i, vweight));
    }

    ll ans = vweight;
    int pos = -1;

    // auto t1 = high_resolution_clock::now();

    contract(1);
    expand(r);

    ll tmp = 0;
    bool flag = false;
    for (int i = 1; i <= n; ++i)
    {
        if (i != r)
        {
            if (in[i].weight == vweight)
            {
                flag = true;
                break;
            }
            tmp += in[i].weight;
        }
    }
    
    // t2 = high_resolution_clock::now();
    // std::cout << "[T] Expansion took " << duration_cast<duration<double>>(t2 - t1).count() << "\n";
    // std::cout << "[T] Loading took " << duration_cast<duration<double>>(accum1).count() << "\n";
    // std::cout << "[T] Dismantling took " << duration_cast<duration<double>>(accum0).count() << "\n";

    if (flag)
        cout << "-1\n";
    else
        cout << tmp << "\n";
    return 0;
}