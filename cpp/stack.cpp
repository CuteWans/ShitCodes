#include <bits/stdc++.h>

using namespace std;

class Stack{
    struct Node {
        int a;
        Node *next;
    };
    Node *tp;
public:
    Stack() {
        tp = new Node;
        tp -> next = nullptr;
        tp -> a = -1;
    }
    Node *newnode(int x) {
        Node *nd = new Node;
        nd -> next = tp;
        nd -> a = x;
        tp = nd;
        return nd;
    }
    int pop() {
        int a = tp -> a;
        auto t = tp -> next;
        delete tp;
        tp = t;
        return a;
    }
    int top() {
        int a = tp -> a;
        return a;
    }
    void push_back(int x) {
        newnode(x);
    }
    bool empty() {
        return tp -> next == nullptr ? 1 : 0;
    }
    void print() {
        for(auto it = tp; it -> next != nullptr; it = it -> next)
            cout << it -> a << ' ';
    }
    ~Stack(){}
};


int main() {
    int n;
    cin >> n;
    Stack stk1, stk2;
    for(int i = 1; i <= n; i ++) {
        int a; cin >> a;
        stk1.push_back(a);
    }
    while(!stk1.empty()) {
        int cur = stk1.pop();
        while(!stk2.empty() && cur < stk2.top()) {
            stk1.push_back(stk2.pop());
        }
        stk2.push_back(cur);
    }
    while(!stk2.empty())
        stk1.push_back(stk2.pop());
    stk1.print();
    return 0;
}