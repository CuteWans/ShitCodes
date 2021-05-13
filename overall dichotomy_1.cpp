//一个数列中多次询问第k小
void solve(int l, int r, vector <Query> q) {
    int mid = (l + r) >> 1;
    if(l == r) {
        for(int i = 0; i < q.size(); i ++)
            ans[q[i].id] = l;
        return ;
    }
    vector <Query> q1, q2;
    for(int i = 0; i < q.size(); i ++) {
        int tmp = check(mid);
        if(q[i].k < tmp)    q1.push_back(q[i]);
        else q[i].k -= tmp, q2.push_back(q[i]);
    }
    solve(l, mid, q1);  solve(mid + 1, r, q2);
    return ;
}