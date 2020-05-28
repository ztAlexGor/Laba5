#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <queue>
#include <ctime>
#include <stack>
#include <string>

using namespace std;

class Node {
	char data;
	int ind;
public:
	Node* left;
	Node* right;
	Node(char a, int ind) {
		data = a;
		left = nullptr;
		right = nullptr;
		this->ind = ind;
	}
	int GetInd() {
		return ind;
	}
	char GetData() {
		return data;
	}
};

class Tree {
public:
	Node* root;
	Tree(string S) {
		root = nullptr;
		AddNode(S[S.size() / 2], S.size() / 2);
		int a = S.size() / 2;
		for (int i = 1; i <= S.size() / 2; i++) { // не успел написать формулировку порядка входа символов в дерево(
			if (a - i >= 0)
				AddNode(S[a - i], a - i);
			if (a + i < S.size())
				AddNode(S[a + i], a + i);
		}
	}
	void AddNode(char a, int ind) {
		if (root == nullptr) {
			root = new Node(a, ind);
			return;
		}
		bool k = true;
		Node* curr = root;
		while (k) {
			if (curr->GetInd() < ind) {
				if (curr->left == nullptr) {
					curr->left = new Node(a, ind);
					k = false;
				}
				else
					curr = curr->left;
			}
			if (curr->GetInd() > ind) {
				if (curr->right == nullptr) {
					curr->right = new Node(a, ind);
					k = false;
				}
				else
					curr = curr->right;
			}
		}
	}
	void InfTraverse(Node* curr) {
		if (curr->left) InfTraverse(curr->left);
		cout << curr->GetData();
		if (curr->right) InfTraverse(curr->right);
	}
	void PreTraverse(Node* curr) {
		cout << curr->GetData();
		if (curr->left) InfTraverse(curr->left);
		if (curr->right) InfTraverse(curr->right);
	}
	void Traverse(Node* curr) {
		cout << curr->GetData();
		if (curr->left) InfTraverse(curr->left);
		if (curr->right) InfTraverse(curr->right);
	}
};

int main() {
	Tree Prim("9+8*(7+(6*(5+4)-(3-2))+1))");
	Prim.InfTraverse(Prim.root);
	cout << endl;
	Prim.PreTraverse(Prim.root);
	return 0;
}