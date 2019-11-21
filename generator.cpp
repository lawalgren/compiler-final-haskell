#include <iostream>
#include <string>
#include "HFunction.cpp"
#include "Parser.cpp"
using namespace std;
ofstream outfile;
string FILE_NAME = "transpiled.cpp"


void preamble() {
    outfile.open(FILE_NAME);
    outfile << "#include <iostream>" << endl << "using namespace std;" << endl << endl << "int main() {" << endl;
}

void postable() {
    outfile << "return 0;" << endl << "}" << endl;
    outfile.close();
}

int main() {
    preamble;

    postamble;
    return 0;
}