#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <queue>
#include <ctime>
#include <stack>
#include <string>

using namespace std;

bool isNumber(string);
float stringToFloat(string);
queue<string> parseToTokens(string);
queue<string> readFromFile(ifstream&);

int main() {
	ifstream input("d:\\Учёба\\Файлы общего доступа\\KOD.txt");

	queue<string> kod = readFromFile(input);
	/*while (!kod.empty()) {
		cout << kod.front() << endl;
		kod.pop();
	}*/

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
		if (curr == "-" || curr == "+" || curr == "*" || curr == "/" || curr == "^" || curr == "=" || curr == "(" || curr == ")" || curr == " " || curr == "}" || curr == "{" || curr == ";") {
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
	queue<string> kod, temp;
	string s;
	while (!inp.eof()) {
		getline(inp, s);
		temp = parseToTokens(s);
		while (!temp.empty()) {
			if (temp.front()!="")kod.push(temp.front());
			temp.pop();
		}
	}
	return kod;
}