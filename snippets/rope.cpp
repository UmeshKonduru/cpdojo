#include <bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct node {
    node *left, *right;
    int weight, size, value;
    node(int v) : left(nullptr), right(nullptr), weight(rng()), size(1), value(v) {}
};

int sz(node *treap) { return treap ? treap->size : 0; }

// left treap constains first k nodes
void split(node *treap, node *&left, node *&right, int k) {
    if(!treap) {
        left = right = nullptr;
        return;
    }
    if(sz(treap->left) < k) {
        split(treap->right,treap->right,right,k);
        left = treap;
    } else {
        split(treap->left,left,treap->left,k);
        right = treap;
    }
    treap->size = sz(treap->left)+sz(treap->right)+1;
}

void merge(node *&treap, node *left, node *right) {
    if(!left) {
        treap = right;
        return;
    }
    if(!right) {
        treap = left;
        return;
    }
    if(left->weight < right->weight) {
        merge(left->right,left->right,right);
        treap = left;
    } else {
        merge(right->left,left,right->left);
        treap = right;
    }
    treap->size = sz(treap->left)+sz(treap->right)+1;
}