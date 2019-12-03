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
    preamble();
    string prog = "solve :: Int -> [String] -> String\n"
                  "solve index [] = \"makes sense\"\n"
                  "solve index (next:rest) = if next == show index || next == \"mumble\"\n"
                  "    then solve (index + 1) rest\n"
                  "    else \"something is fishy\"\n\n"
                  "main :: Void -> Void \n"
                  "main = do\n"
                  "    get_line\n"
                  "    input <- get_line\n"
                  "    let inputlist = words input\n"
                  "    let answer = solve 1 inputlist\n"
                  "    putStrLn answer";
    Parser p = Parser(prog);
    cout << p.functions_list[0].name << endl;
    postamble();
    return 0;
}