/*
* Generator.cpp - Transpiler from Haskell98 to C++
*
*/
#include <iostream>
#include <string>
#include <fstream>
//#include "HFunction.cpp"
#include "Parser.cpp"
using namespace std;
ofstream outfile;
string FILE_NAME = "transpiled.cpp";


void preamble() {
    outfile.open(FILE_NAME);
    if(!outfile.is_open())  cout << "Error Opening output file." << endl;
    outfile << "#include <iostream>" << endl<< "#include <vector>"<< endl
    << "#include <string>" << endl << "using namespace std;" << endl;
}

void function() {

}

void test() {
    ifstream in;
    in.open("test1.txt");
    string progFinal, prog;
    while (!in.eof()) {
        getline(in, prog);
        progFinal += prog + '\n';
    }
    in.close();
    cout << progFinal << endl;
    Parser p = Parser(progFinal);
    cout << p.functions_list[0].name << endl;
}

void postamble() {
    outfile << "return 0;" << endl << "}" << endl;
    outfile.close();
}

int main() {
    preamble();
    //vector<HFunction> funcs;
    cout << "Hello";
    postamble();
    return 0;
}