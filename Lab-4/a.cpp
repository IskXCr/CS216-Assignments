#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <queue>
#include <iomanip>

using namespace std;

struct Point
{
    int idx;
    int parent;
    int rank;
    int x;
    int y;

    Point(int idx, int x, int y) : idx(idx), parent(idx), rank(0), x(x), y(y) {}

    friend ostream &operator<<(ostream &os, const Point &p)
    {
        return os << "Point[" << p.idx << "], position(" << p.x << ", " << p.y << "), parent=point[" << p.parent << "]";
    }
};

vector<Point> points;

double calc_dist(Point &p1, Point &p2)
{
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}


int find(int p)
{
    vector<int> path;
    while (points[p].parent != p)
    {
        path.emplace_back(p);
        p = points[p].parent;
    }
    for (auto i : path)
    {
        points[i].parent = p;
    }
    return p;
}

void link(int p1, int p2)
{
    if (points[p1].rank > points[p2].rank)
        points[p2].parent = p1;
    else
    {
        points[p1].parent = p2;
        if (points[p1].rank == points[p2].rank)
        {
            points[p2].rank += 1;
        }
    }
}

void merge(int p1, int p2)
{
    link(find(p1), find(p2));
}

void print_nodes()
{
    for (auto &i : points)
    {
        cout << "Point[" << i.idx << "], position(" << i.x << ", " << i.y << "), parent=point[" << i.parent << "]\n";
    }
}

struct Edge
{
    Point *p1;
    Point *p2;
    double dist;
    Edge(Point *p1, Point *p2) : p1(p1), p2(p2), dist(calc_dist(*p1, *p2)) {}

    bool is_in_same_cluster() const
    {
        return find(p1->idx) == find(p2->idx);
    }

    friend bool operator<(const Edge &e1, const Edge &e2) { return e1.dist > e2.dist; }
    friend ostream &operator<<(ostream &os, const Edge &edge)
    {
        return os << "Edge with dist=" << edge.dist << ", point1=" << *(edge.p1) << ", point2=" << *(edge.p2) << "\n";
    }
};

priority_queue<Edge> edges;

int main()
{
    cin.tie(nullptr);
    int n, k;
    cin >> n >> k;
    for (int i = 0; i < n; ++i)
    {
        int x, y;
        cin >> x >> y;
        points.emplace_back(Point(i, x, y));
    }
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            edges.push(Edge(&points[i], &points[j]));
        }
    }
    int count = 0;
    while (!edges.empty() && count < n - k)
    {
        Edge edge = edges.top();
        edges.pop();
        if (find(edge.p1->idx) != find(edge.p2->idx))
        {
            count++;
            merge(edge.p1->idx, edge.p2->idx);
        }
    }
    while (!edges.empty() && edges.top().is_in_same_cluster())
    {
        edges.pop();
    }
    cout << fixed << setprecision(2) << edges.top().dist << endl;
    // cout << "Upper edge: " << edges.top() << endl;
    // print_nodes();
    return 0;
}