#include <iostream>
#include <string>
#include <fstream>
#include "HFunction.cpp"
#include "Parser.cpp"
using namespace std;
ofstream outfile;
string FILE_NAME = "transpiled.cpp";


void preamble() {
    outfile.open(FILE_NAME);
    outfile << "#include <iostream>" << endl<< "#include <vector>"<< endl 
    << "#include <string>" << endl << "using namespace std;" << endl;
}



void postamble() {
    outfile << "return 0;" << endl << "}" << endl;
    outfile.close();
}

int main() {
    preamble;

    postamble;
    return 0;
}