#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <chrono>

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
        // std::cout << "Extracting min element from root " << rt << " with edge shown: " << ret << "... ";
        // std::cout << "[Extract-min]: Freeing rt " << rt << " upon edge extraction: " << ret << "\n";
        delete rt;
        rt = merge_heap(left, right);
        // std::cout << "new root = " << rt << "\n";
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
    // std::cout << "\t internal merging " << lhs << " and " << rhs << "\n";
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
    // std::cout << "merging " << lhs.rt << " and " << rhs.rt << "... \n";
    Heap heap;
    heap.rt = merge_heap(lhs.rt, rhs.rt);
    lhs.rt = rhs.rt = nullptr;
    // std::cout << "merged as " << heap.rt << "\n";
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

struct Vertex
{
    Vertex() : id{-1}, in(-1, -1, 0), prev{-1}, parent{-1}, root{-1}, children{}, queue{} {}

    inline void set_parent(int parent)
    {
        this->parent = parent;
        this->root = parent;
    }

    void revert()
    {
        in = bkup_in;
        prev = bkup_prev;
        parent = bkup_parent;
        root = bkup_root;
        children.clear();
        int k = bkup_children.size();
        // children.reserve(k);
        for (int j = 0; j < k; ++j)
            children.push_back(bkup_children[j]);
    }

    int id;
    Edge in;
    int prev;
    int parent;
    int root;

    Edge bkup_in;
    int bkup_prev;
    int bkup_parent;
    int bkup_root;

    std::vector<int> children;
    std::vector<int> bkup_children;
    Heap_Operations::Heap queue;
};

/* Pending: add heap operations */

std::ostream &operator<<(std::ostream &os, const Vertex &v)
{
    os << "vertex (id=" << v.id << ")"
       << ",\tparent=" << v.parent << ",\troot=" << v.root;
    return os;
}

int n, m;
std::vector<Vertex> vertices;

int find(int v)
{
    std::vector<int> list;
    list.reserve(n);
    while (vertices[v].root != -1)
    {
        // std::cout << "finding node " << vertices[v] << "\n";
        list.push_back(v);
        v = vertices[v].root;
    }
    for (auto idx : list)
        vertices[idx].root = v;
    return v;
}

void contract(int a)
{
    while (!vertices[a].queue.empty())
    {
        // std::cout << "queuing node " << vertices[a] << "\n";
        Edge edge = vertices[a].queue.extract_min();

        int b = find(edge.src);

        if (a != b)
        {
            vertices[a].in = edge;
            vertices[a].prev = b;

            if (vertices[edge.src].in.src == -1)
            {
                a = b;
            }
            else // do contraction
            {
                // std::cout << "contracting node " << vertices[a] << "\n";
                vertices.emplace_back(Vertex());
                int c = vertices.size() - 1;
                Vertex &vc = vertices[c];
                vc.id = c;

                for (; a != c; a = find(vertices[a].prev))
                {
                    // std::cout << "contracting node " << vertices[a] << "\n";
                    vertices[a].set_parent(c);
                    /* assign the modified m_weight */
                    vertices[a].queue.modify(-vertices[a].in.m_weight);

                    // edges[vertices[a].in]
                    vc.children.push_back(a);

                    // meld
                    // std::cout << "calling heap merge on " << c << " and " << a << "\n";
                    vc.queue = Heap_Operations::merge_heap(vc.queue, vertices[a].queue);
                }
                a = c;
            }
        }
    }
}

void dismantle(std::queue<int> &pending, int v)
{
    // std::cout << "\tsource dismantle " << v << "\n";
    while (vertices[v].parent != -1)
    {
        v = vertices[v].parent;
        // std::cout << "\t\tdismantle " << v << "\n";
        for (auto i : vertices[v].children)
        {
            vertices[i].parent = -1;
            if (!vertices[i].children.empty())
                pending.push(i);
            // pending.push(i), std::cout << "\t\t\tappending " << i << "\n";
        }
        vertices[v].children.clear();
    }
}

void expand(int root)
{
    // std::cout << "expanding " << root << "\n";
    std::queue<int> pending;
    dismantle(pending, root);

    while (!pending.empty())
    {
        int c = pending.front();
        pending.pop();
        Edge edge = vertices[c].in;
        int v = edge.dst;
        vertices[v].in = edge;
        dismantle(pending, v);
    }
}

void save_all()
{
    for (auto &i : vertices)
    {
        i.bkup_children.clear();
        int k = i.children.size();
        for (int j = 0; j < k; ++j)
            i.bkup_children.push_back(i.children[j]);
        // std::cout << "saving " << k << " children for " << i.id << "\n";
        i.bkup_in = i.in;
        i.bkup_prev = i.bkup_prev;
        i.bkup_parent = i.parent;
        i.bkup_root = i.root;
    }
}

static decltype(std::chrono::high_resolution_clock::now() - std::chrono::high_resolution_clock::now()) accum;

void load_all()
{
    using namespace std::chrono;
    auto t1 = high_resolution_clock::now();
    for (auto &i : vertices)
    {
        i.revert();
        // std::cout << "loading " << i.bkup_children.size() << " children for " << i.id << "\n";
    }
    auto t2 = high_resolution_clock::now();
    accum += (t2 - t1);
}

void clear_all()
{
    for (auto &i : vertices)
    {
        Heap_Operations::release(i.queue);
    }
}

void MDST(int root)
{
    contract(0);
    // std::cout << "contracted\n";
    expand(root);
}

int main()
{
    using namespace std;
    using namespace std::chrono;

    // cin.tie(nullptr);
    // cin.sync_with_stdio(false);

    while (cin >> n >> m)
    {
        vertices.clear();
        ll vweight = 0x7fffffffLL;

        vertices.resize(n + 1);

        for (int i = 0; i <= n; ++i)
            vertices[i].id = i;

        for (int i = 0; i < m; ++i)
        {
            int u, v;
            ll w;
            cin >> u >> v >> w;
            vertices[v + 1].queue.insert(Edge(u + 1, v + 1, w));
        }

        for (int i = 1; i <= n; ++i)
        {
            vertices[i].queue.insert(Edge(i % n + 1, i, vweight));
        }

        ll ans = vweight;
        int pos = -1;

        auto t1 = high_resolution_clock::now();
        contract(1);
        auto t2 = high_resolution_clock::now();
        std::cout << "[T] Contraction took " << duration_cast<duration<double>>(t2 - t1).count() << "\n";
        save_all();
        t1 = high_resolution_clock::now();
        for (int r = 1; r <= n; ++r)
        {
            load_all();
            expand(r);
            ll tmp = 0;
            for (int i = 1; i <= n; ++i)
            {
                if (i != r)
                    tmp += vertices[i].in.weight;
            }
            if (tmp < ans)
            {
                ans = tmp;
                pos = r;
            }
        }
        t2 = high_resolution_clock::now();
        std::cout << "[T] Expansion took " << duration_cast<duration<double>>(t2 - t1).count() << "\n";
        std::cout << "[T] Loading took " << duration_cast<duration<double>>(accum).count() << "\n";

        // std::cout << "Showing contracted edges: \n";
        // load_all();
        // for (int i = 1; i < vertices.size(); ++i)
        // {
        //     cout << "Vertex " << i << ":\t" << vertices[i].in << "\n";
        //     cout << "\tparent " << vertices[i].parent << "\troot" << vertices[i].root << "\n";
        //     cout << "\tshowing childs\n";
        //     for (auto j : vertices[i].children)
        //         cout << "\t\t" << j << "\n";
        // }

        if (ans >= vweight)
            cout << "impossible\n";
        else
            cout << ans << " " << pos - 1 << "\n";

        clear_all();
    }

    // for (int i = 0; i <= n; ++i)
    // cout << vertices[i] << "\n";

    return 0;
}