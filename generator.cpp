/*
* Generator.cpp - Transpiler from Haskell98 to C++
*
*/
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
    if(!outfile.is_open())  cout << "Error Opening output file." << endl;
    outfile << "#include <iostream>" << endl<< "#include <vector>"<< endl
    << "#include <string>" << endl << "using namespace std;" << endl;
}

void function() {

}

Parser getParser() {
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
    return p;
}

void postamble() {
    outfile << "return 0;" << endl << "}" << endl;
    outfile.close();
}

int main() {
    cout << "Preamble Started...";
    preamble();
    cout << "Completed" << endl;


    
    Parser p = getParser();
    //Output function declaration
    for(auto func : p.functions_list ) {
        switch (func.return_type){
            case HFunction::Integer :
                outfile << "int";
            break;
            case HFunction::Float :
                outfile << "float";
            break;
            case HFunction::String :
                outfile << "string";
            break;
            case HFunction::Char :
                outfile << "char";
            break;
            case HFunction::Vector_Integer :
                outfile << "vector<int>";
            break;
            case HFunction::Vector_Float :
                outfile << "vector<float>";
            break;
            case HFunction::Vector_Char :
                outfile << "vector<char>";
            break;
            case HFunction::Vector_String :
                outfile << "vector<string>";
            break;
            case HFunction::Void :
                outfile << "void";
            break;
        }
        outfile << " " << func.name << "(";
        int counter = 0;
        for (auto param : func.params_order) {
            outfile << func.params.at(param) << " " << param;
            if (counter != func.params_order.size() - 1)
                outfile << ", ";
            
            counter++;
        }
        outfile << ") {\n";
        //End function Declaration

        for (auto var : func.where_order) {
            auto rpart = func.where.at(var);
                auto current = &get<0>(rpart);
                stack<HExpression*> stk;
                auto type = get<1>(rpart);
                switch (type){
                    case HFunction::Integer :
                        outfile << "int";
                        break;
                    case HFunction::Float :
                        outfile << "float";
                        break;
                    case HFunction::String :
                        outfile << "string";
                        break;
                    case HFunction::Char :
                        outfile << "char";
                        break;
                    case HFunction::Vector_Integer :
                        outfile << "vector<int>";
                        break;
                    case HFunction::Vector_Float :
                        outfile << "vector<float>";
                        break;
                    case HFunction::Vector_Char :
                        outfile << "vector<char>";
                        break;
                    case HFunction::Vector_String :
                        outfile << "vector<string>";
                        break;
                    case HFunction::Void :
                        outfile << "vector<string>";
                        break;
                }
                outfile << " " << var << " ";
                while (current != nullptr || !stk.empty()) {
                    while (current == nullptr && !stk.empty()) {
                        current = stk.top()->right;
                        stk.pop();
                    }
                    if (current == nullptr)
                        break;
                    stk.push(current);
                    Token tokenDat = current->data;
                    switch( tokenDat.getType()) {
                        case Token::multiplicative_op:
                            if(tokenDat.getContents() != "*" && tokenDat.getContents() != "/") {
                                if(tokenDat.getContents() == "`mod`")
                                    outfile << " % ";
                                else if(tokenDat.getContents() == "`div`")
                                    outfile << " / ";
                            }
                            else {
                                outfile << tokenDat.getContents();
                            }
                            break;
                        case Token::v_empty:
                            outfile << "{}";
                            break;
                        case Token::v_h_char:
                        case Token::v_h_string:
                        case Token::v_h_int:
                        case Token::v_h_float:
                            outfile << "{" << tokenDat.getContents().substr(1,tokenDat.getContents().length() - 2) << "}";
                            break;
                        default:
                            outfile << tokenDat.getContents();
                    }
                    //outfile << current->data.getContents()
                }

        }


    }


    //vector<HFunction> funcs;
    
    postamble();
    return 0;
}