#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;
const int N = 2e5 + 1;

int n;
vector<int> adj[N];
queue<int> q;
int dist[N], it[N];
int match[N][2];

bool bfs() {
    for(int u = 1; u <= n; u++) {
        if(match[u][0]) dist[u] = INF;
        else {
            dist[u] = 0;
            q.push(u);
        }
    }
    bool found = false;
    while(!q.empty()) {
        int u = q.front(); q.pop();
        for(int v : adj[u]) {
            int w = match[v][1];
            if(w == 0) found = true;
            else if(dist[w] == INF) {
                dist[w] = dist[u]+1;
                q.push(w);
            }
        }
    }
    return found;
}

bool dfs(const int u) {
    for(int &i = it[u]; i < adj[u].size(); ++i) {
        int v = adj[u][i];
        int w = match[v][1];
        if(w == 0 || (dist[w] == dist[u]+1 && dfs(w))) {
            match[u][0] = v;
            match[v][1] = u;
            return true;
        }
    }
    dist[u] = INF;
    return false;
}

void hopcroftkarp() {
    while(bfs()) {
        memset(it+1,0,n*sizeof(int));
        for(int u = 1; u <= n; u ++) {
            if(match[u][0] == 0) dfs(u);
        }
    }
}