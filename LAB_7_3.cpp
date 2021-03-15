#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Node {
    int key;
    int height;
    int balance;
    int num;
    Node* left;
    Node* right;
    Node* parent;
    Node() {
        key = -1;
        height = 1;
        balance = 0;
        num = 0;
        left = right = parent = nullptr;
    }
};

int findSize(Node* p) {
    return p ? p->height : 0;
}

int balance_factor(Node* p) {
    return findSize(p->right) - findSize(p->left);
}

void correctHeight(Node* p) {
    int hl = findSize(p->left);
    int hr = findSize(p->right);
    p->height = max(hl, hr) + 1;
}

Node* rotateRight(Node* p) {
    Node* q = p->left;
    p->left = q->right;
    q->right = p;
    if (!p->parent) {
        q->parent = nullptr;
    }
    else {
        q->parent = p->parent;
        if (q->right == q->parent->left) {
            p->parent->left = q;
        }
        if (q->right == q->parent->right) {
            p->parent->right = q;
        }
    }
    correctHeight(p);
    correctHeight(q);
    q->balance = balance_factor(q);
    return q;
}

Node* rotateLeft(Node* p) {
    Node* q = p->right;
    p->right = q->left;
    q->left = p;
    if (!p->parent) {
        q->parent = nullptr;
    }
    else {
        q->parent = p->parent;
        if (q->left == q->parent->left) {
            p->parent->left = q;
        }
        if (q->left == q->parent->right) {
            p->parent->right = q;
        }
    }
    correctHeight(p);
    correctHeight(q);
    q->balance = balance_factor(q);
    return q;
}

Node* balance(Node* p) {
    correctHeight(p);
    p->balance = balance_factor(p);
    if (p->balance == 2) {
        if (p->right->balance < 0) {
            p->right = rotateRight(p->right);
        }
        p = rotateLeft(p);
    }
    if (p->balance == -2) {
        if (p->left->balance > 0) {
            p->left = rotateLeft(p->left);
        }
        p = rotateRight(p);
    }
    if (p->parent == nullptr) {
        return p;
    }
    balance(p->parent);
}

void connect(Node* p, int x, vector<Node>& N) {
    if (p->key < x) {
        if (p->right) {
            connect(p->right, x, N);
        }
        else {
            p->right = &N[N.size() - 1];
            N[N.size() - 1].parent = p;
        }
    }
    if (p->key > x) {
        if (p->left) {
            connect(p->left, x, N);
        }
        else {
            p->left = &N[N.size() - 1];
            N[N.size() - 1].parent = p;
        }
    }
}

int main() {
    ifstream fin("addition.in");
    ofstream fout("addition.out");
    int N;
    fin >> N;
    fout << N + 1 << "\n";
    vector<Node> nodes_arr(N + 1);

    for (int i = 0; i < N; i++) {
        int key;
        int left_node;
        int right_node;
        fin >> key >> left_node >> right_node;
        nodes_arr[i].key = key;
        nodes_arr[i].num = i + 1;
        if (left_node) {
            nodes_arr[i].left = &nodes_arr[left_node - 1];
            nodes_arr[left_node - 1].parent = &nodes_arr[i];
        }
        if (right_node) {
            nodes_arr[i].right = &nodes_arr[right_node - 1];
            nodes_arr[right_node - 1].parent = &nodes_arr[i];
        }
    }

    int x;
    fin >> x;
    nodes_arr[N].key = x;
    nodes_arr[N].num = N + 1;
    connect(&nodes_arr[0], x, nodes_arr);

    for (int i = N; i >= 0; i--) {
        int h = 0;
        if (nodes_arr[i].right) {
            nodes_arr[i].balance += nodes_arr[i].right->height;
            h = max(h, nodes_arr[i].right->height);
        }
        if (nodes_arr[i].left) {
            nodes_arr[i].balance -= nodes_arr[i].left->height;
            h = max(h, nodes_arr[i].left->height);
        }
        nodes_arr[i].height = h + 1;
    }

    queue<int> Queue_out;
    int first;
    if (nodes_arr.size() != 1) {
        first = balance(nodes_arr[N].parent)->num;
    }
    else {
        first = 1;
    }
    Queue_out.push(first);
    int count = 2;
    while (!Queue_out.empty()) {
        int cur = Queue_out.front() - 1;
        Queue_out.pop();
        fout << nodes_arr[cur].key << " ";
        if (nodes_arr[cur].left) {
            Queue_out.push(nodes_arr[cur].left->num);
            fout << count << " ";
            count++;
        }
        else {
            fout << "0 ";
        }
        if (nodes_arr[cur].right) {
            Queue_out.push(nodes_arr[cur].right->num);
            fout << count << "\n";
            count++;
        }
        else {
            fout << "0\n";
        }
    }
}