#include<bits/stdc++.h>

const int N =100005;

using namespace std;

int n, m;
string str[N];

struct Node {
    int v, nex;
    Node(int v = 0, int nex = 0) :
        v(v), nex(nex) { }
} Edge[N << 4];

int stot = 1, h[N << 1], f[N << 4];
void add(int u, int v, int w) {
    Edge[++ stot] = Node(v, h[u]);
    h[u] = stot;
    f[stot] = w;

    Edge[++ stot] = Node(u, h[v]);
    h[v] = stot;
    f[stot] = 0;
}

bool vis[N];
int dis[N];
queue <int> q;
int s, t;
bool bfs() {
    memset(vis, 0, sizeof(vis));
    memset(dis, 0, sizeof(dis));
    q.push(s);    vis[s] = 1;
    while(!q.empty()) {
        int u = q.front();    q.pop();
        for(int i = h[u]; i; i = Edge[i].nex) {
            int v = Edge[i].v;
            if(!vis[v] && f[i]) {
                q.push(v);
                vis[v] = 1;
                dis[v] = dis[u] + 1;
            }
        }
    }
    return vis[t];
}

int dfs(int x, int delta) {
    if(x == t)    return delta;
    int res = 0;
    for(int i = h[x]; i; i = Edge[i].nex) {
        int v = Edge[i].v;
        if(f[i] && dis[v] == dis[x] + 1) {
            int dd = dfs(v, min(delta, f[i]));
            f[i] -= dd;
            f[i ^ 1] += dd;
            delta -= dd;
            res += dd;
        }
    }
    if(res == 0)    dis[x] = 0;
    return res;
}

int cy[N];
bool find(int u) {
    vis[u] = 1;
    for(int i = h[u]; i; i = Edge[i].nex) {
        int v = Edge[i].v;
        if(v == s || v == t)    continue ;
        if(!vis[v]) {
            vis[v] = 1;
            if(!cy[v] || find(cy[v])) {
                cy[v] = u;
                return 1;
            }
        }
    }
    return 0;
}

int G[N], flag[N], pos[N];
map <int, int> mp;
int main() {
    cin >> n >> m;
    for(int i = 1; i <= n; i ++)
        cin >> str[i];
    
    t = n * m + 1;

    int sum = 0;
    for(int i = 1; i <= n; i ++) {
        for(int j = 1; j <= m; j ++) {
            int now = (i - 1) * m + j;
            if(i >= 2 && j == 1)    pos[now] = pos[now - m] ^ 1;
            else pos[now] = pos[now - 1] ^ 1;
            if(str[i][j - 1] == '.')    sum ++, G[now] = 1;
            if(str[i][j - 1] == '.' && pos[now]) {
                add(0, now, 1);
                if(i - 1 >= 1 && str[i - 1][j - 1] == '.')
                    add(now, now - m, 1);
                if(i + 1 <= n && str[i + 1][j - 1] == '.')
                    add(now, now + m, 1);
                if(j - 1 >= 1 && str[i][j - 2] == '.')
                    add(now, now - 1, 1);
                if(j + 1 <= m && str[i][j] == '.')
                    add(now, now + 1, 1);
            } else if(str[i][j - 1] == '.' && !pos[now]) add(now, t, 1);
        }
    }

    int ans = 0;
    while(bfs())
        ans += dfs(s, 0x3f3f3f3f);
    cout << sum - ans << endl;

    for(int i = 2; i <= stot; i += 2)
        if(Edge[i].v != 0 && Edge[i].v != t && Edge[i ^ 1].v != 0 && Edge[i ^ 1].v != t && f[i ^ 1] != 0) {
            mp[Edge[i].v] = Edge[i ^ 1].v;
            mp[Edge[i ^ 1].v] = Edge[i].v;
            cy[Edge[i].v] = Edge[i ^ 1].v;
        }

    for(int i = 1; i <= n * m; i ++)
        if(G[i] && !mp[i])  flag[i] = 1;
    for(int i = 1; i <= n * m; i ++)
        if(flag[i] && pos[i] && G[i])
            find(i);
    for(int i = 1; i <= n * m; i ++)
        if(G[i]) {
            if(pos[i] && vis[i])           flag[i] = 1;
            else if(!pos[i] && !vis[i])    flag[i] = 1;
        }
    for(int i = 1; i <= n; i ++) {
        for(int j = 1; j <= m; j ++) {
            if(str[i][j - 1] == '*')    cout << '*';
            else if(flag[(i - 1) * m + j])  cout << 'C';
            else cout << '.';
        }
        cout << endl;
    }
    return 0;
}