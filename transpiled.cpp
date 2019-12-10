#include <iostream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

vector<string> words(string input) { 
	vector<string> result; 
	istringstream iss(s); 
	for(string s: iss; iss >> s; ) 
		result.push_back(s);    
 return result; 
}

void putStrLn(string line) { 
	 cout << line << endl;
}

void getLine(string &line) {
	 getline(cin, line); 
}

void getLine() {
	 string temp;
	 getline(cin, temp);
}

string solve(int index, vector<string> empty) {
}

string solve(int index, vector<string> pattern) {
	string next = pattern[0];
	vector<string> rest = pattern.erase(pattern.begin(), pattern.begin()+1);
}

int main() {
	getLine();
	getLine(input);
	vector<string> inputlist = words(input);
	int answer = solve(inputlist, 1);
	putStrLn(answer);
	;
	return 0;
}
