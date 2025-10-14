#include <bits/stdc++.h>
using namespace std;

const int INF = 1e18;

// iterative implementation
struct segtree {

    int n;
    vector<int> t;

    segtree() {}
    segtree(int n) { resize(n); }
    segtree(const vector<int>& a) { build(a); }

    int f(const int &a, const int &b) {
        return max(a,b);
    }

    void resize(int n) {
        this->n = n;
        t.assign(2*n,0);
    }

    void build(const vector<int>& a) {
        n = a.size();
        t.resize(2*n);
        for(int i = 0; i < n; i++) t[n+i] = a[i];
        for(int i = n-1; i > 0; i--) t[i] = f(t[i<<1],t[i<<1|1]);
    }

    void update(int p, int x) {
        for(t[p += n] = x; p > 1; p >>= 1) t[p>>1] = f(t[p],t[p^1]);
    }

    int query(int l, int r) {
        int res = -INF;
        for(l += n, r += n; l <= r; l >>= 1, r >>= 1) {
            if(l&1) res = f(res,t[l++]);
            if(!(r&1)) res = f(res,t[r--]);
        }
        return res;
    }
};

// recursive implementation template
struct segtree {

    int n;
    vector<int> t;

    segtree() {}
    segtree(int n) { resize(n); }
    segtree(const vector<int>& a) { build(a); }

    void resize(int n) {
        this->n = n;
        t.assign(4*n,0);
    }

    void build(const vector<int>& a, int v, int tl, int tr) {
        if(tl == tr) {
            t[v] = a[tl];
            return;
        }
        int tm = (tl+tr) / 2;
        build(a,2*v,tl,tm); build(a,2*v+1,tm+1,tr);
        t[v] = t[2*v] + t[2*v+1];
    }

    void update(int v, int tl, int tr, int i, int val) {
        if(tl == tr) {
            t[v] = val;
            return;
        }
        int tm = (tl+tr)/2;
        if(i > tm) update(2*v+1,tm+1,tr,i,val);
        else update(2*v,tl,tm,i,val);
        t[v] = t[2*v] + t[2*v+1];
    }

    int query(int v, int tl, int tr, int l, int r) {
        if(l > r) return 0;
        if(l == tl && r == tr) return t[v];
        int tm = (tl+tr) / 2;
        return query(2*v,tl,tm,l,min(r,tm))
                + query(2*v+1,tm+1,tr,max(l,tm+1),r);
    }

    void build(const vector<int>& a) { n = a.size(); t.assign(4*n,0); build(a,1,0,n-1); }
    void update(int i, int val) { update(1,0,n-1,i,val); }
    int query(int l, int r) { return query(1,0,n-1,l,r); }
};

// lazy add, lazy assign, range sum
struct segtree {

    int n;
    vector<int> t, lz_add, lz_set;
    vector<bool> flag;

    segtree() {}
    segtree(int n) { resize(n); }
    segtree(const vector<int>& a) { build(a); }

    void resize(int n) {
        this->n = n;
        t.assign(4*n,0);
        lz_add.assign(4*n,0);
        lz_set.resize(4*n);
        flag.assign(4*n,0);
    }

    void build(const vector<int>& a, int v, int tl, int tr) {
        lz_add[v] = flag[v] = 0;
        if(tl == tr) {
            t[v] = a[tl];
            return;
        }
        int tm = (tl+tr) / 2;
        build(a,2*v,tl,tm); build(a,2*v+1,tm+1,tr);
        t[v] = t[2*v] + t[2*v+1];
    }

    void push(int v, int tl, int tr) {
        if(tl == tr) return;
        int tm = (tl+tr) / 2;
        int l_sz = tm-tl+1, r_sz = tr-tm;
        int lc = 2*v, rc = 2*v+1;
        if(flag[v]) {
            flag[v] = 0;
            t[lc] = lz_set[v]*l_sz, t[rc] = lz_set[v]*r_sz;
            lz_set[lc] = lz_set[rc] = lz_set[v];
            lz_add[lc] = lz_add[rc] = 0;
            flag[lc] = flag[rc] = 1;
        }
        if(lz_add[v]) {
            t[lc] += lz_add[v]*l_sz, t[rc] += lz_add[v]*r_sz;
            lz_add[lc] += lz_add[v], lz_add[rc] += lz_add[v];
            lz_add[v] = 0;
        }
    }

    void add(int v, int tl, int tr, int l, int r, int val) {
        if(l > r) return;
        if(l == tl && r == tr) {
            t[v] += val*(tr-tl+1);
            lz_add[v] += val;
            return;
        }
        push(v,tl,tr);
        int tm = (tl+tr) / 2;
        add(2*v,tl,tm,l,min(r,tm),val);
        add(2*v+1,tm+1,tr,max(l,tm+1),r,val);
        t[v] = t[2*v] + t[2*v+1];
    }

    void assign(int v, int tl, int tr, int l, int r, int val) {
        if(l > r) return;
        if(l == tl && r == tr) {
            t[v] = val*(tr-tl+1);
            lz_set[v] = val;
            lz_add[v] = 0;
            flag[v] = 1;
            return;
        }
        push(v,tl,tr);
        int tm = (tl+tr) / 2;
        assign(2*v,tl,tm,l,min(r,tm),val);
        assign(2*v+1,tm+1,tr,max(l,tm+1),r,val);
        t[v] = t[2*v] + t[2*v+1];
    }

    int query(int v, int tl, int tr, int l, int r) {
        if(l > r) return 0;
        if(l == tl && r == tr) return t[v];
        push(v,tl,tr);
        int tm = (tl+tr) / 2;
        return query(2*v,tl,tm,l,min(r,tm))
                + query(2*v+1,tm+1,tr,max(l,tm+1),r);
    }

    void build(const vector<int>& a) {
        n = a.size();
        t.resize(4*n);
        lz_add.resize(4*n);
        lz_set.resize(4*n);
        flag.resize(4*n);
        build(a,1,0,n-1);
    }

    void add(int l, int r, int val) { add(1,0,n-1,l,r,val); }
    void assign(int l, int r, int val) { assign(1,0,n-1,l,r,val); }
    int query(int l, int r) { return query(1,0,n-1,l,r); }
};

// lazy assign + range max
struct segtree {

    int n;
    vector<int> t, lz;
    vector<bool> flag;

    segtree() {}
    segtree(int n) { resize(n); }
    segtree(const vector<int>& a) { build(a); }

    void resize(int n) {
        this->n = n;
        t.assign(4*n,0);
        lz.assign(4*n,0);
        flag.assign(4*n,0);
    }

    void build(const vector<int>& a, int v, int tl, int tr) {
        lz[v] = flag[v] = 0;
        if(tl == tr) {
            t[v] = a[tl];
            return;
        }
        int tm = (tl+tr) / 2;
        build(a,2*v,tl,tm); build(a,2*v+1,tm+1,tr);
        t[v] = max(t[2*v],t[2*v+1]);
    }

    void push(int v, int tl, int tr) {
        if(tl == tr || !flag[v]) return;
        lz[2*v] = lz[2*v+1] = lz[v];
        t[2*v] = t[2*v+1] = lz[v];
        flag[2*v] = flag[2*v+1] = 1;
        flag[v] = 0;
    }

    void assign(int v, int tl, int tr, int l, int r, int val) {
        if(l > r) return;
        if(l == tl && r == tr) {
            t[v] = val;
            lz[v] = val;
            flag[v] = 1;
            return;
        }
        push(v,tl,tr);
        int tm = (tl+tr) / 2;
        assign(2*v,tl,tm,l,min(r,tm),val);
        assign(2*v+1,tm+1,tr,max(l,tm+1),r,val);
        t[v] = max(t[2*v],t[2*v+1]);
    }

    int query(int v, int tl, int tr, int l, int r) {
        if(l > r) return -INF;
        if(l == tl && r == tr) return t[v];
        push(v,tl,tr);
        int tm = (tl+tr) / 2;
        return max(query(2*v,tl,tm,l,min(r,tm)),
                   query(2*v+1,tm+1,tr,max(l,tm+1),r));
    }

    void build(const vector<int>& a) {
        n = a.size();
        t.resize(4*n);
        lz.resize(4*n);
        flag.resize(4*n);
        build(a,1,0,n-1);
    }

    void assign(int l, int r, int val) { assign(1,0,n-1,l,r,val); }
    int query(int l, int r) { return query(1,0,n-1,l,r); }
};

// lazy add, lazy assign, range max
struct segtree {

    int n;
    vector<int> t, lz_add, lz_set;
    vector<bool> flag;

    segtree() {}
    segtree(int n) { resize(n); }
    segtree(const vector<int>& a) { build(a); }

    void resize(int n) {
        this->n = n;
        t.assign(4*n,0);
        lz_add.assign(4*n,0);
        lz_set.resize(4*n);
        flag.assign(4*n,0);
    }

    void build(const vector<int>& a, int v, int tl, int tr) {
        lz_add[v] = flag[v] = 0;
        if(tl == tr) {
            t[v] = a[tl];
            return;
        }
        int tm = (tl+tr) / 2;
        build(a,2*v,tl,tm); build(a,2*v+1,tm+1,tr);
        t[v] = max(t[2*v],t[2*v+1]);
    }

    void push(int v, int tl, int tr) {
        if(tl == tr) return;
        int lc = 2*v, rc = 2*v+1;
        if(flag[v]) {
            flag[v] = 0;
            t[lc] = t[rc] = lz_set[v];
            lz_set[lc] = lz_set[rc] = lz_set[v];
            lz_add[lc] = lz_add[rc] = 0;
            flag[lc] = flag[rc] = 1;
        }
        if(lz_add[v]) {
            t[lc] += lz_add[v], t[rc] += lz_add[v];
            lz_add[lc] += lz_add[v], lz_add[rc] += lz_add[v];
            lz_add[v] = 0;
        }
    }

    void add(int v, int tl, int tr, int l, int r, int val) {
        if(l > r) return;
        if(l == tl && r == tr) {
            t[v] += val;
            lz_add[v] += val;
            return;
        }
        push(v,tl,tr);
        int tm = (tl+tr) / 2;
        add(2*v,tl,tm,l,min(r,tm),val);
        add(2*v+1,tm+1,tr,max(l,tm+1),r,val);
        t[v] = max(t[2*v],t[2*v+1]);
    }

    void assign(int v, int tl, int tr, int l, int r, int val) {
        if(l > r) return;
        if(l == tl && r == tr) {
            t[v] = val;
            lz_set[v] = val;
            lz_add[v] = 0;
            flag[v] = 1;
            return;
        }
        push(v,tl,tr);
        int tm = (tl+tr) / 2;
        assign(2*v,tl,tm,l,min(r,tm),val);
        assign(2*v+1,tm+1,tr,max(l,tm+1),r,val);
        t[v] = max(t[2*v],t[2*v+1]);
    }

    int query(int v, int tl, int tr, int l, int r) {
        if(l > r) return -INF;
        if(l == tl && r == tr) return t[v];
        push(v,tl,tr);
        int tm = (tl+tr) / 2;
        return max(query(2*v,tl,tm,l,min(r,tm))
                    ,query(2*v+1,tm+1,tr,max(l,tm+1),r));
    }

    void build(const vector<int>& a) {
        n = a.size();
        t.resize(4*n);
        lz_add.resize(4*n);
        lz_set.resize(4*n);
        flag.resize(4*n);
        build(a,1,0,n-1);
    }

    void add(int l, int r, int val) { add(1,0,n-1,l,r,val); }
    void assign(int l, int r, int val) { assign(1,0,n-1,l,r,val); }
    int query(int l, int r) { return query(1,0,n-1,l,r); }
};

// lazy assign, range min, sticky
struct segtree {

    int n;
    vector<int> t, lz;
    vector<bool> flag;

    segtree() {}
    segtree(int n) { resize(n); }

    void resize(int n) {
        this->n = n;
        t.assign(4*n,INF);
        lz.assign(4*n,INF);
        flag.assign(4*n,0);
    }

    void push(int v, int tl, int tr) {
        if(tl == tr || !flag[v]) return;
        lz[2*v] = min(lz[2*v],lz[v]);
        lz[2*v+1] = min(lz[2*v+1],lz[v]);
        t[2*v] = min(t[2*v],lz[v]);
        t[2*v+1] = min(t[2*v+1],lz[v]);
        flag[2*v] = flag[2*v+1] = 1;
        flag[v] = 0, lz[v] = INF;
    }

    void assign(int v, int tl, int tr, int l, int r, int val) {
        if(l > r) return;
        if(l == tl && r == tr) {
            t[v] = min(t[v],val);
            lz[v] = min(lz[v],val);
            flag[v] = 1;
            return;
        }
        push(v,tl,tr);
        int tm = (tl+tr) / 2;
        assign(2*v,tl,tm,l,min(r,tm),val);
        assign(2*v+1,tm+1,tr,max(l,tm+1),r,val);
        t[v] = min(t[2*v],t[2*v+1]);
    }

    int query(int v, int tl, int tr, int l, int r) {
        if(l > r) return INF;
        if(l == tl && r == tr) return t[v];
        push(v,tl,tr);
        int tm = (tl+tr) / 2;
        return min(query(2*v,tl,tm,l,min(r,tm)),
                   query(2*v+1,tm+1,tr,max(l,tm+1),r));
    }

    void assign(int l, int r, int val) { assign(1,0,n-1,l,r,val); }
    int query(int l, int r) { return query(1,0,n-1,l,r); }
};