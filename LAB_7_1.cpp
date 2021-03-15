#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
	int key;
	int height;
	int balance;
	Node* left;
	Node* right;
	Node() { // конструктор для новых узлов дерева
		key = -1;
		height = 1;
		balance = 0;
		left = right = 0;
	}
};

int main() {
	ifstream fin("balance.in");
	ofstream fout("balance.out");
	int N;
	fin >> N;
	vector <Node> nodes_arr(N);

	for (int i = 0; i < N; i++) { // цикл записи заданных значений в массив
		int key;
		int left_node;
		int right_node;
		fin >> key >> left_node >> right_node;
		nodes_arr[i].key = key;
		if (left_node) {
			nodes_arr[i].left = &nodes_arr[left_node - 1];
		}
		if (right_node) {
			nodes_arr[i].right = &nodes_arr[right_node - 1];
		}
	}
	for (int i = N - 1; i >= 0; i--) { // цикл нахождения балансировки
		int h = 0;
		if (nodes_arr[i].right) {
			nodes_arr[i].balance += nodes_arr[i].right->height;
			h = max(h, nodes_arr[i].right->height);
		}
		if (nodes_arr[i].left){
			nodes_arr[i].balance -= nodes_arr[i].left->height;
			h = max(h, nodes_arr[i].left->height);
		}
		nodes_arr[i].height = h + 1;
	}
	for (int i = 0; i < N; i++) {
		fout << nodes_arr[i].balance << "\n";
	}
	return 0;
}