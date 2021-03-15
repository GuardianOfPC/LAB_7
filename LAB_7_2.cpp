#include <fstream>
#include <vector>
#include <queue>

using namespace std;

struct Node {
	int key;
	int height;
	int balance;
	int num;
	Node* left;
	Node* right;
	Node() {
		key = -1;
		height = 1;
		balance = 0;
		num = 0;
		left = right = 0;
	}
};

int findSize(Node* p) {
	if (!p)
		return 0;
	else
		return p->height;
}

int max(int x, int y) {
	return x > y ? x : y;
}

void fixHeight(Node* p) {
	int height_left = findSize(p->left);
	int height_right = findSize(p->right);
	p->height = max(height_left, height_right) + 1;
}

Node* rotateLeft(Node* p) {
	Node* q = p->right;
	p->right = q->left;
	q->left = p;
	fixHeight(p);
	fixHeight(q);
	return q;
}

Node* rotateRight(Node* p) {
	Node* q = p->left;
	p->left = q->right;
	q->right = p;
	fixHeight(p);
	fixHeight(q);
	return q;
}

int balance(Node* p) {
	if (p->right->balance < 0) {
		p->right = rotateRight(p->right);
	}
	p = rotateLeft(p);
	return p->num;
}

int main() {
	ifstream fin("rotation.in");
	ofstream fout("rotation.out");
	int N;
	fin >> N;
	fout << N << "\n";
	vector <Node> nodes_arr(N);

	for (int i = 0; i < N; i++) {
		int key;
		int left_node;
		int right_node;
		fin >> key >> left_node >> right_node;
		nodes_arr[i].key = key;
		nodes_arr[i].num = i + 1;
		if (left_node) {
			nodes_arr[i].left = &nodes_arr[left_node - 1];
		}
		if (right_node) {
			nodes_arr[i].right = &nodes_arr[right_node - 1];
		}
	}

	for (int i = N - 1; i >= 0; i--) {
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

	queue<int> Queue;
	int first_element = balance(&nodes_arr[0]);
	Queue.push(first_element);
	int count = 2;
	while (!Queue.empty()) {
		int current = Queue.front() - 1;
		Queue.pop();
		fout << nodes_arr[current].key << " ";
		if (nodes_arr[current].left) {
			Queue.push(nodes_arr[current].left->num);
			fout << count << " ";
			count++;
		}
		else
			fout << "0 ";
		if (nodes_arr[current].right) {
			Queue.push(nodes_arr[current].right->num);
			fout << count << "\n";
			count++;
		}
		else
			fout << "0\n";
	}
	return 0;
}