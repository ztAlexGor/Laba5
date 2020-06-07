#include <iostream>
#include <fstream>
#include <cmath>
#include <queue>
#include <string>
#include <map>
#include <vector>
#include <conio.h>

using namespace std;

bool isNumber(string);
float stringToFloat(string);
vector<string> parseToTokens(string);
queue<string> readFromFile(ifstream&);
class StatementList;
class Tree; 
class BinTree;
class IfTree;

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
	void SetData(string data) {
		this->data = data;
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
	void Output() {
		cout << data << endl;
		if (left)
			left->Output();
		if (right)
			right->Output();
	}
};

class Tree {
protected:
	Tree* parent;
public:
	virtual float Count(Node* start) = 0;
	virtual Node* GetHead() = 0;
	virtual void SetParent(Tree* parent) {
		this->parent = parent;
	}
	virtual float GetNumber(string s) = 0;
	virtual void AddStatment(string key, float value) = 0;
	virtual void Output(Node* root) = 0;
	Tree* GetParent() {
		return parent;
	}
};

class BinTree : public Tree {
	bool IsState;
	Node* head;
public:
	BinTree(string ryad) {
		vector<string> tokens = parseToTokens(ryad);
		AddNode(head, tokens);
	}
	void AddNode(Node*& ptr, vector<string> tokens) {
		if (tokens.size() == 1) {
			ptr = new Node(tokens[0]);
		}
		else {
			int brackets = 0;
			int ind, prior = 5;
			for (int i = tokens.size() - 1; i >= 0; i--) {
				if (tokens[i] == ")") {
					brackets++;
					continue;
				}
				if (tokens[i] == "(") {
					brackets--;
					continue;
				}
				if (brackets == 0 && prior > 1 && tokens[i] == "=") {
					prior = 1;
					ind = i;
				}
				if (brackets == 0 && prior > 2 && (tokens[i] == "+" || tokens[i] == "-")) {
					prior = 2;
					ind = i;
				}
				if (brackets == 0 && prior > 3 && (tokens[i] == "*" || tokens[i] == "/")) {
					prior = 3;
					ind = i;
				}
				if (brackets == 0 && prior > 4 && tokens[i] == "^") {
					prior = 4;
					ind = i;
				}
			}
			ptr = new Node(tokens[ind]);
			if (head == nullptr)
				head = ptr;
			vector<string> left, right;
			for (int i = 0; i < tokens.size(); i++) {
				if (i < ind)
					left.push_back(tokens[i]);
				if (i > ind)
					right.push_back(tokens[i]);
			}
			bool w = true;
			while (w) {
				if (left[0] == "(" && left[left.size() - 1] == ")") {
					left.pop_back();
					for (int i = 0; i < left.size() - 1; i++)
						left[i] = left[i + 1];
					left.pop_back();
				}
				else
					w = false;
			}
			while (!w) {
				if (right[0] == "(" && right[right.size() - 1] == ")") {
					right.pop_back();
					for (int i = 0; i < right.size() - 1; i++)
						right[i] = right[i + 1];
					right.pop_back();
				}
				else
					w = true;
			}
			tokens.clear();
			AddNode(ptr->left, left);
			AddNode(ptr->right, right);
		}
	}
	Node* GetHead() {
		return head;
	}
	float Count(Node* start) {
		if (!start->left) {
			if (start->isNumber())
				return start->GetNumber();
			return parent->GetNumber(start->GetSymbol());
		}
		if (start->GetSymbol() == "=") {
			parent->AddStatment(start->left->GetSymbol(), Count(start->right));
		}
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
	bool IsItState() {
		return IsState;
	}
	void Output(Node* start) override {
		start->Output();
	}
	void AddStatment(string key, float value) override {

	}
	float GetNumber(string s) override {
		return 0;
	}
};

class IfTree : public Tree {
	Node* head;
	BinTree* Condition;
	BinTree* True;
	BinTree* False;
public:
	IfTree(string cond, string yes, string no) {
		head = nullptr;
		Condition = new BinTree(cond);
		Condition->SetParent(parent);
		True = new BinTree(yes);
		True->SetParent(parent);
		False = new BinTree(no);
		False->SetParent(parent);
	}
	float Count(Node* none) override {
		if (Condition->Count(Condition->GetHead()) != 0)
			return True->Count(True->GetHead());
		else
			return False->Count(False->GetHead());
	}
	void Output(Node* start) override {
		Condition->Output(Condition->GetHead());
		cout << endl << "/*/" << endl;
		if (True)
			True->Output(True->GetHead());
		cout << endl << "/*/" << endl;
		if (False)
			False->Output(False->GetHead());
	}
	Node* GetHead() {
		return head;
	}
	void AddStatment(string key, float value) override {

	}
	float GetNumber(string s) override {
		return 0;
	}
	void SetParent(Tree* parent) override {
		this->parent = parent;
		Condition->SetParent(parent);
		True->SetParent(parent);
		False->SetParent(parent);
	}
};

class StatementList : public Tree {
	map<string, float> statments;
	vector<Tree*> StTrees;
public:
	StatementList(queue<string> ryad) {
		parent = this;
		while (!ryad.empty()) {
			if (ryad.front()[0] == 'i' && ryad.front()[1] == 'f' && ryad.front()[2] == '(') {
				string cond, yes, no;
				int etap = 0;
				for (int i = 0; i < ryad.front().size(); i++) {
					if (ryad.front()[i] == '(' || ryad.front()[i] == '{')
						etap++;
					if (etap == 1 && ryad.front()[i] != '\n' && ryad.front()[i] != ')' && ryad.front()[i] != '}' && ryad.front()[i] != '{' && ryad.front()[i] != '(' && ryad.front()[i] != ';')
						cond += ryad.front()[i];
					if (etap == 2 && ryad.front()[i] != '\n' && ryad.front()[i] != ')' && ryad.front()[i] != '}' && ryad.front()[i] != '{' && ryad.front()[i] != '(' && ryad.front()[i] != ';')
						if (ryad.front()[i] == 'e' && ryad.front()[i + 1] == 'l' && ryad.front()[i + 2] == 's' && ryad.front()[i + 3] == 'e')
							i += 4;
						else
							yes += ryad.front()[i];
					if (etap == 3 && ryad.front()[i] != '\n' && ryad.front()[i] != ')' && ryad.front()[i] != '}' && ryad.front()[i] != '{' && ryad.front()[i] != '(' && ryad.front()[i] != ';')
						no += ryad.front()[i];
				}
				ryad.pop();
				Tree* New = new IfTree(cond, yes, no);
				StTrees.push_back(New);
				New->SetParent(this);
			}
			else {
				Tree* New = new BinTree(ryad.front());
				ryad.pop();
				StTrees.push_back(New);
				New->SetParent(this);
			}

		}
	}
	float GetNumber(string key) {
		return statments[key];
	}
	void AddStatment(string key, float value) {
		statments[key] = value;
	}
	float Count(Node* none) override {
		for (int i = 0; i < StTrees.size(); i++) {
			if (i == StTrees.size() - 1)
				cout << "Result = " << StTrees[i]->Count(StTrees[i]->GetHead()) << ";";
			else
				StTrees[i]->Count(StTrees[i]->GetHead());
		}
		return 0;
	}
	void Output(Node* none) override {
		for (int i = 0; i < StTrees.size(); i++) {
			StTrees[i]->Output(StTrees[i]->GetHead());
			cout << endl << "/////////////" << endl;
		}
	}
	Node* GetHead() override {
		return nullptr;
	}
	Tree* GetParentOf(int i) {
		return StTrees[i]->GetParent();
	}
};

int main() {
	ifstream input;
	input.open("D:\\Учёба\\Файлы общего доступа\\KOD.txt");
	queue<string> kod;
	kod = readFromFile(input);
	/*while (!kod.empty()) {
		cout << kod.front() << endl << "//////////////////////////////////" << endl;
		kod.pop();
	}*/
	StatementList Lab(kod);
	//Lab.Output(nullptr);
	Lab.Count(nullptr);
	_getch();
}

bool isNumber(string s) {
	for (int i = 0; i < s.size(); i++) {
		if (((int)s[i] < 48 || (int)s[i] > 57) && s[i] != '.')return false;
	}
	return true;
}

float stringToFloat(string s) {
	float part1 = 0, part2 = 0;
	int it = 0;
	while (it < s.size() && s[it] != '.') {
		part1 *= 10;
		part1 += (int)s[it] - 48;
		it++;
	}
	if (it != s.size()) {
		for (int i = s.size() - 1; i > it; i--) {
			part2 += (int)s[i] - 48;
			part2 /= 10;
		}
	}
	return part1 + part2;
}

vector<string> parseToTokens(string s) {
	vector<string> res;
	string token = "";
	for (int i = 0; i < s.size(); i++) {
		string curr = "";
		curr += s[i];
		if (curr == "-" || curr == "+" || curr == "*" || curr == "/" || curr == "^" || curr == "=" || curr == "(" || curr == ")" || curr == " " /*|| curr == "}" || curr == "{" || curr == ";"*/) {
			if (token != "")res.push_back(token);
			if (curr != " ")res.push_back(curr);
			token = "";
		}
		else {
			token += curr;
			/*if (token == "if") {
			  res.push(token);
			  token = "";
			}*/
		}
	}
	res.push_back(token);
	if (res[res.size() - 1][res[res.size() - 1].size() - 1] == ';')
		res[res.size() - 1].erase(res[res.size() - 1].size() - 1);
	/*for (int i = 0; i < res.size(); i++) {
		cout << res[i] << endl;
	}*/
	return res;
}

queue<string> readFromFile(ifstream& inp) {
	queue<string> kod;
	string s;
	while (!inp.eof()) {
		getline(inp, s);
		if (s[0] == '\n')
			s.erase(0, 1);
		if (s[0] == 'i' && s[1] == 'f' && s[2] == '(') {
			string g;
			while (g != "}") {
				getline(inp, g);
				s += g;
			}
			g.clear();
			while (g != "}") {
				getline(inp, g);
				s += g;
			}
		}
		if (!s.empty())
			kod.push(s);
	}
	return kod;
}