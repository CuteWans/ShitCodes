#include <bits/stdc++.h>

using namespace std;

const int N = 4000000 + 10;
int MAXN, a[N];
int n, m;

inline int read () {
    int x = 0, f = 0; char c = getchar ();
    while (!isdigit (c)) f |= (c == '-'), c = getchar ();
    while (isdigit (c)) x = x * 10 + c - '0', c = getchar ();
    return f ? -x : x;
}

struct Splay {
    int son[N][2], fat[N], key[N], siz[N], ntot, root[N];

    inline void update( int nd ) {
        siz[nd] = siz[son[nd][0]] + siz[son[nd][1]] + 1;
    }
    void rotate( int u, int nd, int d ) {
        int p = fat[nd];
        int s = son[nd][!d];
        int ss = son[s][d];

        if( p ) son[p][ nd==son[p][1] ] = s;
        else root[u]=s;
        son[nd][!d] = ss;
        son[s][d] = nd;

        fat[nd] = s;
        fat[s] = p;
        if( ss ) fat[ss]=nd;

        update(nd);
        update(s);
    }
    void splay( int u, int nd, int top=0 ) {
        while( fat[nd]!=top ) {
            int p=fat[nd];
            int nl=nd==son[p][0];
            if( fat[p]==top ) {
                rotate( u, p, nl );
            } else {
                int pp=fat[p];
                int pl=p==son[pp][0];
                if( nl==pl ) {
                    rotate( u, pp, pl );
                    rotate( u, p, nl );
                } else {
                    rotate( u, p, nl );
                    rotate( u, pp, pl );
                }
            }
        }
    }
    int newnode( int p, int k ) {
        int nd = ++ntot;
        key[nd] = k;
        siz[nd] = 1;
        fat[nd] = p;
        son[nd][0] = son[nd][1] = 0;
        return nd;
    }
    void insert( int pos, int v ) {
        if( !root[pos] ) {
            root[pos] = newnode( 0, v );
            return;
        }
        int nd = root[pos];
        while( son[nd][v>key[nd]] ) nd=son[nd][v>key[nd]];
        son[nd][v>key[nd]] = newnode( nd, v );
        update(nd);
        splay(pos, nd);
    }
    int find( int u, int v ) {
        int nd = root[u];
        while( key[nd]!=v ) nd=son[nd][ v>key[nd] ];
        return nd;
    }
    void erase( int u, int v ) {
        int nd = find(u, v);
        splay(u, nd);
        int lnd = son[nd][0];
        int rnd = son[nd][1];
        if( !lnd && !rnd ) {
            root[u] = 0;
        } else if( !lnd ) {
            root[u] = rnd;
            fat[rnd] = 0;
        } else if( !rnd ) {
            root[u] = lnd;
            fat[lnd] = 0;
        } else {
            while( son[lnd][1] ) lnd=son[lnd][1];
            while( son[rnd][0] ) rnd=son[rnd][0];
            splay( u, lnd, 0 );
            splay( u, rnd, lnd );
            son[rnd][0] = 0;
            update(rnd);
             update(lnd);
        }
    }
    int nth( int u, int k ) {
        if(k == 0)  return -2147483647;
        if(siz[root[u]] < k)    return 2147483647;
        int nd=root[u];
        while(1) {
            int lz = siz[son[nd][0]];
            if( k<=lz ) {
                nd = son[nd][0];
            } else if( k>=lz+2 ) {
                k -= lz+1;
                nd = son[nd][1];
            } else {
                splay(u, nd);
                return key[nd];
            }
        }
    }
    int rank( int u, int v ) {
        int nd=root[u];
        int rt = 1;
        int last_nd;
        while(nd) {
            last_nd = nd;
            if( key[nd]<v ) {
                rt += siz[son[nd][0]] + 1;
                nd = son[nd][1];
            } else {
                nd = son[nd][0];
            }
        }
        splay(u, last_nd);
        return rt;
    }
    int prev( int u, int v ) {
        int k = rank(u, v);
        return nth(u, k-1);
    }
    int succ( int u, int v ) {
        int k = rank(u, v+1);
        return nth(u, k);
    }
}T;

inline void seginsert(int nd, int l, int r, int pos, int val) {
    T.insert(nd, val);
    if(l == r)  return ;
    int mid = (l + r) >> 1;
    if(pos <= mid)  seginsert(nd << 1, l, mid, pos, val);
    else    seginsert(nd << 1 | 1, mid + 1, r, pos, val);
}

inline int segrank(int nd, int l, int r, int L, int R, int k) {
    if(l == L && r == R)    return T.rank(nd, k);
    int mid = (l + r) >> 1;
    if(R <= mid)    return segrank(nd << 1, l, mid, L, R, k);
    if(L > mid)     return segrank(nd << 1 | 1, mid + 1, r, L, R, k);
    return segrank(nd << 1, l, mid, L, mid, k) + segrank(nd << 1 | 1, mid + 1, r, mid + 1, R, k) - 1;
}

inline int segkth(int L, int R, int k) {
    int l = 0, r = MAXN + 1;
    while(l <= r) {
        int mid = (l + r) >> 1;
        int tmp = segrank(1, 1, n, L, R, mid);
        if(tmp > k)    r = mid - 1;
        else l = mid + 1;
    }
    return l - 1;
}

inline void modify(int nd, int l, int r, int pos, int k) {
    T.erase(nd, a[pos]);
    T.insert(nd, k);
    if(l == r) {
        a[pos] = k;
        return ;
    }
    int mid = (l + r) >> 1;
    if(pos <= mid)    modify(nd << 1, l, mid, pos, k);
    else    modify(nd << 1 | 1, mid + 1, r, pos, k);
}

inline int segpre(int nd, int l, int r, int L, int R, int k) {
    if(l == L && r == R)    return T.prev(nd, k);
    int mid = (l + r) >> 1;
    if(R <= mid)    return segpre(nd << 1, l, mid, L, R, k);
    if(L > mid)     return segpre(nd << 1 | 1, mid + 1, r, L, R, k);
    return max(segpre(nd << 1, l, mid, L, mid, k), segpre(nd << 1 | 1, mid + 1, r, mid + 1, R, k));
}

inline int seglas(int nd, int l, int r, int L, int R, int k) {
    if(l == L && r == R)    return T.succ(nd, k);
    int mid = (l + r) >> 1;
    if(R <= mid)    return seglas(nd << 1, l, mid, L, R, k);
    if(L > mid)     return seglas(nd << 1 | 1, mid + 1, r, L, R, k);
    return min(seglas(nd << 1, l, mid, L, mid, k), seglas(nd << 1 | 1, mid + 1, r, mid + 1, R, k));
}

int main() {
    scanf("%d%d", &n, &m);
    for(int i = 1; i <= n; i ++) {
        a[i] = read();
        seginsert(1, 1, n, i, a[i]);
        MAXN = max(MAXN, a[i]);
    }
    for(int i = 1; i <= m; i ++) {
        int opt, l, r, k, pos;
        opt = read();
        if(opt == 1) {
            l = read(), r = read(), k = read();
            printf("%d\n", segrank(1, 1, n, l, r, k));
        } else if(opt == 2) {
            l = read(), r = read(), k = read();
            printf("%d\n", segkth(l, r, k));
        } else if(opt == 3) {
            pos = read(), k = read();
            modify(1, 1, n, pos, k);
        } else if(opt == 4) {
            l = read(), r = read(), k = read();
            printf("%d\n", segpre(1, 1, n, l, r, k));
        } else {
            l = read(), r = read(), k = read();
            printf("%d\n", seglas(1, 1, n, l, r, k));
        }
    }
}