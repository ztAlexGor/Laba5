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

bool isNumber(string);
float stringToFloat(string);
queue<string> parseToTokens(string);
queue<string> readFromFile(ifstream&);


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

class StatementList {
	map<string, float> statments;
	vector<Tree*> StTrees;
public:
	StatementList(queue<string> ryad) {
		while (!ryad.empty()) {
			if (ryad.front()[0] == 'i' && ryad.front()[1] == 'f' && ryad.front()[2] == '(') {
				string cond, yes, no;
				int etap = 0;
				for (int i = 0; i < ryad.front().size(); i++) {
					if (ryad.front()[i] == '(' || ryad.front()[i] == '{')
						etap++;
					if (etap == 1 && ryad.front()[i] != '\n' && ryad.front()[i] != ')' && ryad.front()[i] != '}' && ryad.front()[i] != '{')
						cond += ryad.front()[i];
					if (etap == 2 && ryad.front()[i] != '\n' && ryad.front()[i] != ')' && ryad.front()[i] != '}' && ryad.front()[i] != '{')
						yes += ryad.front()[i];
					if (etap == 3 && ryad.front()[i] != '\n' && ryad.front()[i] != ')' && ryad.front()[i] != '}' && ryad.front()[i] != '{')
						no += ryad.front()[i];
				}
				Tree* New = new IfTree(cond, yes, no);
			}

		}
	}
	float GetNumber(string key) {
		return statments[key];
	}
	void AddStatment(string key, float value) {
		statments[key] = value;
	}
	void Count() {
		for (int i = 0; i < StTrees.size(); i++) {
			if (i = StTrees.size() - 1)
				cout << "Result = " << StTrees[i]->Count(StTrees[i]->GetHead()) << ".";
			else
				StTrees[i]->Count(StTrees[i]->GetHead());
		}
	}
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




int main() {
	ifstream input;
	input.open("d:\\Учёба\\Файлы общего доступа\\KOD.txt");
	string s;
	queue<string> kod;
	kod = readFromFile(input);
	StatementList Lab(kod);
	return 0;
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

queue<string> parseToTokens(string s) {
	queue<string> res;
	string token = "";
	for (int i = 0; i < s.size(); i++) {
		string curr = "";
		curr += s[i];
		if (curr == "-" || curr == "+" || curr == "*" || curr == "/" || curr == "^" || curr == "=" || curr == "(" || curr == ")" || curr == " " /*|| curr == "}" || curr == "{" */|| curr == ";") {
			if (token != "")res.push(token);
			if (curr != " ")res.push(curr);
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
	res.push(token);
	return res;
}

queue<string> readFromFile(ifstream& inp) {
	queue<string> kod;
	string s;
	while (!inp.eof()) {
		getline(inp, s,';');
		if (s[0] == '\n')
			s.erase(0, 1);
		if (s[0] == 'i' && s[1] == 'f' && s[2] == '(') {
			string g;
			getline(inp, g, ';');
			s += g;
			getline(inp, g, '}');
			s += g;
			s += "}";
		}
		kod.push(s);
	}
	return kod;
}