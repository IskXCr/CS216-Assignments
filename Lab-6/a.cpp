#include <iostream>
#include <vector>

using std::cin, std::cout;

constexpr int ubd = 30010;
int n, m, t;
int num = 0;
int cache[ubd];
int lastupd[ubd];
int queueidx[ubd]; // the index of last insertion
int queueupd[ubd]; // the update time of last insertion
int qhead = 0, qtail = 0;

void remove(int time)
{
    while (qhead != qtail && queueupd[qhead] < time - t)
    {
        if (queueupd[qhead] == lastupd[queueidx[qhead]])
        {
            // cout << "evicting " << queueidx[qhead] << " as it expires: " << queueupd[qhead] << "\n";
            cache[queueidx[qhead]] = -1;
            num--;
        }
        qhead = (qhead + 1) % ubd;
    }
}

void update(int time, int key, int val)
{
    bool flag = false;
    if (cache[key] == -1)
        flag = true;
    if (flag && num == n) // if then eject one of them
    {
        while (queueupd[qhead] != lastupd[queueidx[qhead]])
            qhead = (qhead + 1) % ubd;
        cache[queueidx[qhead]] = -1;
        qhead = (qhead + 1) % ubd;
        num--;
    }
    queueidx[qtail] = key;
    queueupd[qtail] = time;
    cache[key] = val;
    lastupd[key] = time;
    qtail = (qtail + 1) % ubd;
    if (flag)
        num++;
}

void print_cache()
{
    cout << "in_queue:\n";
    int q = qhead;
    while (q != qtail)
    {
        cout << "\t[" << queueidx[q] << "]=" << cache[queueidx[q]] << ", queueupd=" << queueupd[q] << "\n";
        q = (q + 1) % ubd;
    }
}

void access(int time, int key)
{
    cout << cache[key] << "\n";
    if (cache[key] != -1)
        update(time, key, cache[key]);
}

int main()
{
    // cin.tie(nullptr);
    // cin.sync_with_stdio(false);
    for (int i = 0; i < ubd; ++i)
        cache[i] = -1;

    cin >> n >> m >> t;
    for (int i = 0; i < m; ++i)
    {
        remove(i);
        int op;
        cin >> op;
        if (op == 1)
        {
            int k;
            cin >> k;
            access(i, k);
        }
        else if (op == 2)
        {
            int k, v;
            cin >> k >> v;
            update(i, k, v);
        }
        // cout << "after iter " << i << ":\n";
        // print_cache();
    }
    while (qhead != qtail)
    {
        while (qhead != qtail && queueupd[qhead] != lastupd[queueidx[qhead]])
            qhead = (qhead + 1) % ubd;
        if (qhead == qtail)
            break;
        cout << cache[queueidx[qhead]] << " ";
        qhead = (qhead + 1) % ubd;
    }
    cout << "\n";
    return 0;
}