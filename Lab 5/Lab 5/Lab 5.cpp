#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <queue>
#include <ctime>
#include <stack>
#include <string>
#include <map>
#include <vector>

using namespace std;

class Node {
	string data;
public:
	Node* left;
	Node* right;
	Node(string a) {
		data = a;
		left = nullptr;
		right = nullptr;
	}
	bool isNumber() {
		for (int i = 0; i < data.size(); i++) {
			if (((int)data[i] < 48 || (int)data[i] > 57) && data[i] != '.')
				return false;
		}
		return true;
	}

	string GetSymbol() {
		return data;
	}

	float GetNumber() {
		float part1 = 0, part2 = 0;
		int it = 0;
		while (it < data.size() && data[it] != '.') {
			part1 *= 10;
			part1 += (int)data[it] - 48;
			it++;
		}
		if (it != data.size()) {
			for (int i = data.size() - 1; i > it; i--) {
				part2 += (int)data[i] - 48;
				part2 /= 10;
			}
		}
		return part1 + part2;
	}
};

class Tree {
	Node* head;
public:
	virtual float Count(Node* start) {}
	virtual Node* GetHead() {}
};



class BinTree : public Tree {
	bool IsState;
	StatementList* parent;
	Node* head;
public:
	BinTree(string ryad) {
		/////////////Stroenie dereva is ryadka///////////////
	}
	Node* GetHead() {
		return head;
	}
	float Count(Node* start) {
		if (!start->left) {
			if (start->isNumber)
				return start->GetNumber();
			return parent->GetNumber(start->GetSymbol());
		}
		if (start->GetSymbol() == "=")
			return Count(start->right);
		if (start->GetSymbol() == "+")
			return Count(start->left) + Count(start->right);
		if (start->GetSymbol() == "-")
			return Count(start->left) - Count(start->right);
		if (start->GetSymbol() == "*")
			return Count(start->left) * Count(start->right);
		if (start->GetSymbol() == "/")
			return Count(start->left) / Count(start->right);
		if (start->GetSymbol() == "^")
			return pow(Count(start->left), Count(start->right));
	}
	void SetState(bool k) {
		IsState = k;
	}
	bool IsState() {
		return IsState;
	}
};

class IfTree : public Tree {
	BinTree* Condition;
	BinTree* True;
	BinTree* False;
public:
	IfTree(string cond, string yes, string no) {
		Condition = new BinTree(cond);
		True = new BinTree(yes);
		False = new BinTree(no);
	}
	float Count() {
		if (Condition->Count(Condition->GetHead()) != 0)
			return True->Count(True->GetHead());
		else
			return False->Count(False->GetHead());
	}
};

class StatementList {
	map<string, float> statments;
	vector<Tree*> StTrees;
public:
	StatementList(string file) {
		ifstream fin;
		fin.open(file);
		string ForAdd;
		//////////////////////Tut delenie na ryadki i IF'i || zapihivanie ih v vector/////////////////////
	}
	float GetNumber(string key) {
		return statments[key];
	}
	void Count() {
		for (int i = 0; i < StTrees.size(); i++) {
			if (i = StTrees.size() - 1)
				cout << "Result = " << StTrees[i] << ".";
			else
				statments[StTrees[i]->GetHead()->left->GetSymbol()] = StTrees[i]->Count(StTrees[i]->GetHead()->right);
		}
	}
};

