#include <bits/stdc++.h>
using namespace std;

struct sparsetable {

    int n,k;
    vector<vector<int>> st;

    sparsetable() {}
    sparsetable(const vector<int>& a) { build(a); }

    int f(const int &a, const int &b) {
        return min(a,b);
    }

    int log2_f(int x) {
        if(x == 0) return -1;
        return 63 - __builtin_clzll(x);
    }

    void build(const vector<int>& a) {
        n = a.size();
        k = log2_f(n);
        st.assign(k+1,vector<int>(n));
        copy(a.begin(),a.end(),st[0].begin());
        for(int i = 1; i <= k; i++) {
            for(int j = 0; j + (1ll << i) <= n; j++) {
                st[i][j] = f(st[i-1][j],st[i-1][j+(1ll<<(i-1))]);
            }
        }
    }

    int query(int l, int r) {
        int i = log2_f(r-l+1);
        return f(st[i][l],st[i][r-(1ll<<i)+1]);
    }
};