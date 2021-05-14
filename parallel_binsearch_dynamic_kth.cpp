#include <bits/stdc++.h>

const int N = 300005;

using namespace std;

int n, m, ans[N], tot;

struct Node {
    int id, l, r, x, y, k, pos;
    Node(int id = 0, int l = 0, int r = 0, int x = 0, int y = 0, int k = 0, int pos = 0) :
        id(id), l(l), r(r), x(x), y(y), k(k), pos(pos) { }
} Q[N], q1[N], q2[N];

int tr[N];
int lowbit(int x) {
    return x & (-x);
}

void add(int nd, int v) {
    for(int i = nd; i <= n; i += lowbit(i))
        tr[i] += v;
}

int query(int nd) {
    int ans = 0;
    for(int i = nd; i; i -= lowbit(i))
        ans += tr[i];
    return ans;
}

void solve(int l, int r, int L, int R)
// 当前的值域范围为 [l,r], 处理的操作的区间为 [L,R]
{
     if(l > r || L > R) return;
    int cnt1 = 0, cnt2 = 0, m = (l + r) / 2;
    // cnt1, cnt2 分别为分到左边, 分到右边的操作数
    if(l == r) {
        for(int i = L; i <= R; i++)
            if(Q[i].id == 1)    ans[Q[i].pos] = l;
        return;
    }
    for(int i = L; i <= R; i++)
        if(Q[i].id == 1) {  // 是询问: 进行分类
            int t = query(Q[i].r) - query(Q[i].l - 1);
            if(Q[i].k <= t)
                q1[++cnt1] = Q[i];
            else
                Q[i].k -= t, q2[++cnt2] = Q[i];
        } else
            // 是修改: 更新树状数组 & 分类
            if(Q[i].y <= m)
                add(Q[i].x, Q[i].k), q1[++cnt1] = Q[i];
            else
                q2[++cnt2] = Q[i];
    for (int i = 1; i <= cnt1; i++)
        if(q1[i].id == 2)  add(q1[i].x, -q1[i].k);  // 清空树状数组
    for (int i = 1; i <= cnt1; i++) Q[L + i - 1] = q1[i];
    for (int i = 1; i <= cnt2; i++)
        Q[L + cnt1 + i - 1] = q2[i];  // 将临时数组中的元素合并回原数组
    solve(l, m, L, L + cnt1 - 1), solve(m + 1, r, L + cnt1, R);
    return;
}

int a[N];
int main() {
    int cnt = 0;
    cin >> n >> m;
    for(int i = 1; i <= n; i ++) {
        cin >> a[i];
        Q[++ tot] = Node(2, 0, 0, i, a[i], 1, tot);
    }
    for(int i = 1; i <= m; i ++) {
        char opt;
        cin >> opt;
        if(opt == 'Q') {
            int l, r, k;
            cin >> l >> r >> k;
            Q[++ tot] = Node(1, l, r, 0, 0, k, ++ cnt);
        } else {
            int x, y;
            cin >> x >> y;
            Q[++ tot] = Node(2, 0, 0, x, a[x], -1, tot);
            a[x] = y;
            Q[++ tot] = Node(2, 0, 0, x, y, 1, tot);
        }
    }
    solve(0, 1e9, 1, tot);
    for(int i = 1; i <= cnt; i ++)
        cout << ans[i] << endl;
    return 0;
}