#include <bits/stdc++.h>

const int N = 10000;

using namespace std;

int n, K, q;
int app[100010][21], ans[100005];

int tot1, tot2;
struct QAQ {
    int u, v, t, k, id;
    QAQ(int u = 0, int v = 0, int t = 0, int k = 0, int id = 0) :
        u(u), v(v), t(t), k(k), id(id) { }
} q1[100005], q2[100005];

struct Node {
    int ls, rs;
    int siz;
} tr[15000000];
int root[11][100005];

void update(int nd) {
    tr[nd].siz = tr[tr[nd].ls].siz + tr[tr[nd].rs].siz;
}

int tail;
int build(int l, int r) {
    int nd = ++ tail;
    if(l == r)  return nd;
    int mid = (l + r) >> 1;
    tr[nd].ls = build(l, mid);
    tr[nd].rs = build(mid + 1, r);
    return nd;
}

int insert(int nd, int l, int r, int pos) {
    int nnd = ++ tail;
    if(l == r) {
        tr[nnd].siz = tr[nd].siz + 1;
        return nnd;
    }
    int mid = (l + r) >> 1;
    if(pos <= mid) {
        tr[nnd].rs = tr[nd].rs;
        tr[nnd].ls = insert(tr[nd].ls, l, mid, pos);
    } else {
        tr[nnd].ls = tr[nd].ls;
        tr[nnd].rs = insert(tr[nd].rs, mid + 1, r, pos);
    }
    update(nnd);
    return nnd;
}

int query(int lnd, int rnd, int l, int r, int k) {
    if(l == r)    return l;
    int x = tr[tr[rnd].ls].siz - tr[tr[lnd].ls].siz;
    int mid = (l + r) >> 1;
    if(x >= k)  return query(tr[lnd].ls, tr[rnd].ls, l, mid, k);
    else        return query(tr[lnd].rs, tr[rnd].rs, mid + 1, r, k - x);
}

int main() {
    scanf("%d%d%d", &n, &K, &q);
    for(int i = 1; i <= n; i ++)
        for(int j = 1; j <= K; j ++) 
            scanf("%d", app[i] + j);
            
    for(int k = 1; k <= K; k ++)
        root[k][0] = build(1, N);
    for(int k = 1; k <= K; k ++)
        for(int i = 1; i <= n; i ++) {
            int pos = ((k + i - 1) % K + K) % K;
            if(!pos)    pos = K;
            root[k][i] = insert(root[k][i - 1], 1, N, app[i][pos]);
        }
    for(int i = 1; i <= q; i ++) {
        int u, v, t, k;
        scanf("%d%d%d%d", &u, &v, &t, &k);
        if(u <= v) {
            q1[++tot1] = QAQ(u, v, t, k, i);
        } else {
            q2[++tot2] = QAQ(u, v, t, k, i);
            
        }
    }
    for(int i = 1; i <= tot1; i ++) {
        int t = q1[i].t, u = q1[i].u, v = q1[i].v, k = q1[i].k;
        int t1 = (((t - u + 1) % K) + K) % K;
        if(!t1) t1 = K;
        ans[q1[i].id] = query(root[t1][u - 1], root[t1][v], 1, N, k);
    }
    tail = 0;
    for(int k = 1; k <= K; k ++)
        root[k][n + 1] = build(1, N);
    for(int k = 1; k <= K; k ++)
        for(int i = n; i >= 1; i --) {
            int pos = ((k + n - i) % K + K) % K;
            if(!pos)    pos = K;
            root[k][i] = insert(root[k][i + 1], 1, N, app[i][pos]);
        }
    for(int i = 1; i <= tot2; i ++) {
        int t = q2[i].t, u = q2[i].u, v = q2[i].v, k = q2[i].k;
        int t2 = ((t - n + u) % K + K) % K;
        if(!t2) t2 = K;
        ans[q2[i].id] = query(root[t2][u + 1], root[t2][v], 1, N, k);
    }
    for(int i = 1; i <= q; i ++)
        printf("%d\n", ans[i]);
    return 0;
}