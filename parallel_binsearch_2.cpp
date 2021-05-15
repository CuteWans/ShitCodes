//多区间询问第k小
void solve(int l, int r, vector <Num> a, vector <Query> q) {
    int mid = (l + r) >> 1;
    if(l == r) {
        for(int i = 0; i < q.size(); i ++)  ans[q[i].id] = l;
        return ;
    }
    vector <Num> a1, a2;
    vector <Query> q1, q2;
    for(int i = 0; i < a.size(); i ++)
        if(a[i].x <= mid)  
            a1.push_back(a[i]), add(a[i].pos, 1);
        else  
            a2.push_back(a[i]);
    for(int i = 0; i < q.size(); i ++) {
        int tmp = query(q[i].r) - query(q[i].l - 1);
        if(q[i].k <= t) q1.push_back(q[i]);
        else   q[i].k -= tmp, q2.push_back(q[i]);
    }
    for(int i = 0; i < a1.size(); i ++) add(a1[i].pos, -1);
    solve(l, mid, a1, q1);  solve(mid + 1, r, a2, q2);
    return ;
}
