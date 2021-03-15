#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

struct Node
{
    int key;
    int height;
    Node* left;
    Node* right;
};

unsigned char findHeight(Node* p) {
    return p ? p->height : 0;
}

int balance_factor(Node* p) {
    return findHeight(p->right) - findHeight(p->left);
}

void correctHeight(Node* p) {
    unsigned char hl = findHeight(p->left);
    unsigned char hr = findHeight(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
}

Node* rotateleft(Node* q) {
    Node* p = q->right;
    q->right = p->left;
    p->left = q;
    correctHeight(q);
    correctHeight(p);
    return p;
}

Node* rotateright(Node* p) {
    Node* q = p->left;
    p->left = q->right;
    q->right = p;
    correctHeight(p);
    correctHeight(q);
    return q;
}

Node* balance(Node* p) {
    correctHeight(p);
    if (balance_factor(p) == 2) {
        if (balance_factor(p->right) < 0) {
            p->right = rotateright(p->right);
        }
        return rotateleft(p);
    }
    if (balance_factor(p) == -2) {
        if (balance_factor(p->left) > 0) {
            p->left = rotateleft(p->left);
        }
        return rotateright(p);
    }
    return p;
}

Node* findMax(Node* p) {
    return p->right ? findMax(p->right) : p;
}

Node* removeMax(Node* p) {
    if (p->right == nullptr) {
        return p->left;
    }
    p->right = removeMax(p->right);
    return balance(p);
}

Node* insert(Node* tree, Node* p) {
    if (!tree) {
        tree = new Node;
        tree->key = p->key;
        tree->left = tree->right = nullptr;
        tree->height = 1;
        return tree;
    }
    if (p->key < tree->key) {
        tree->left = insert(tree->left, p);
    }
    else {
        tree->right = insert(tree->right, p);
    }
    return balance(tree);
}

Node* remove(Node* p, int k) {
    if (!p) {
        return nullptr;
    }
    if (k < p->key) {
        p->left = remove(p->left, k);
    }
    else if (k > p->key) {
        p->right = remove(p->right, k);
    }
    else
    {
        Node* q = p->left;
        Node* r = p->right;
        p = nullptr;
        if (!q) {
            return r;
        }
        Node* t = findMax(q);
        t->left = removeMax(q);
        t->right = r;
        return balance(t);
    }
    return balance(p);
}

int main() {
    ifstream fin("deletion.in");
    ofstream fout("deletion.out");
    int N;
    int X;
    int L;
    int R;
    fin >> N;
    fout << N - 1 << "\n";
    vector<Node> nodes_arr(N);
    Node* tree = nullptr;
    for (int i = 0; i < N; i++) {
        fin >> X >> L >> R;
        nodes_arr[i].key = X;
        if (L) {
            nodes_arr[i].left = &nodes_arr[L - 1];
        }
        if (R) {
            nodes_arr[i].right = &nodes_arr[R - 1];
        }
    }
    queue<Node*> Queue;
    Queue.push(&nodes_arr[0]);
    while (!Queue.empty()) {
        Node* current = Queue.front();
        Queue.pop();
        tree = insert(tree, current);
        if (current->left) {
            Queue.push(current->left);
        }
        if (current->right) {
            Queue.push(current->right);
        }
    }
    fin >> X;
    tree = remove(tree, X);
    if (!tree) {
        return 0;
    }
    Queue.push(tree);
    int count = 2;
    while (!Queue.empty()) {
        Node* current = Queue.front();
        Queue.pop();
        fout << current->key << " ";
        if (current->left) {
            Queue.push(current->left);
            fout << count << " ";
            count++;
        }
        else {
            fout << "0 ";
        }
        if (current->right) {
            Queue.push(current->right);
            fout << count << "\n";
            count++;
        }
        else {
            fout << "0\n";
        }
    }
    return 0;
}