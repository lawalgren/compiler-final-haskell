<<<<<<< HEAD
/*
* Generator.cpp - Transpiler from Haskell98 to C++
* 
*/
=======
#pragma once
>>>>>>> fa2176e83e0d396889a889d0cd0552a14bc3a8e9
#include <iostream>
#include <string>
#include <fstream>
//#include "HFunction.cpp"
//#include "Parser.cpp"
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