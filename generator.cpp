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

void iterate_expression(HExpression* current, Parser p);
void typing(HFunction::Type type);

    void preamble() {
    outfile.open(FILE_NAME);
    if(!outfile.is_open())  cout << "Error Opening output file." << endl;
    outfile << "#include <iostream>" << endl<< "#include <vector>"<< endl << "#include <sstream>" << endl
    << "#include <string>" << endl << "using namespace std;" << endl;


    outfile <<  "\nvector<string> words(string input) { \n"
                "\tvector<string> result; \n\tistringstream iss(s); \n"  
                "\tfor(string s: iss; iss >> s; ) \n"
                    "\t\tresult.push_back(s);    \n return result; \n}\n\n";

    outfile <<  "void putStrLn(string line) { \n"
                "\t cout << line << endl;\n}\n\n";

    outfile <<  "void getLine(string &line) {\n"
                "\t getline(cin, line); \n}\n\n";

    outfile << "void getLine() {\n"
               "\t string temp;\n"
               "\t getline(cin, temp);\n}\n\n";
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
    outfile << "\treturn 0;" << endl << "}" << endl;
    outfile.close();
}

int main() {
    cout << "Preamble Started...";
    preamble();
    cout << "Completed" << endl;


    
    Parser p = getParser();

    //Output function declaration
    for(auto func : p.functions_list ) {
        switch (func.return_type) {
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
        string patternArr[2];
        outfile << " " << func.name << "(";
        int counter = 0;
        for (auto param : func.params_order) {
            switch(func.params.at(param)) {
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
            if(param == "[]")
                outfile << " " << "empty";
            
            else if(param[0] == '(') {
                outfile << " " << "pattern";
                bool colon = false;
                string one = "", two = "";
                for(auto ch: param) {
                    if(ch == ':') colon = true;
                    if(ch != '(' && ch != ')' && ch != ':') {
                        if(!colon) {
                            one += ch;
                        } 
                        else 
                            two += ch;
                    }
                }
                patternArr[0] = one;
                patternArr[1] = two;
            }
            else
                outfile << " " << param;
            if (counter != func.params_order.size() - 1)
                outfile << ", ";
            
            counter++;
        }
        outfile << ") {\n";

        if(func.purity) {
            //End function Declaration

            if(patternArr[0] != "") {
                outfile << "\tstring " << patternArr[0] << " = pattern[0];\n";
                outfile << "\tvector<string> " << patternArr[1] << " = pattern.erase(pattern.begin(), pattern.begin()+1);\n"; 
            }

            for (auto var : func.where_order) {
                auto rpart = func.where.at(var.first);
                    auto current = &get<0>(rpart);
                    stack<HExpression*> stk;
                    auto type = get<1>(rpart);
                    switch (type){
                        case HFunction::Integer :
                            outfile << "\tint";
                            break;
                        case HFunction::Float :
                            outfile << "\tfloat";
                            break;
                        case HFunction::String :
                            outfile << "\tstring";
                            break;
                        case HFunction::Char :
                            outfile << "\tchar";
                            break;
                        case HFunction::Vector_Integer :
                            outfile << "\tvector<int>";
                            break;
                        case HFunction::Vector_Float :
                            outfile << "\tvector<float>";
                            break;
                        case HFunction::Vector_Char :
                            outfile << "\tvector<char>";
                            break;
                        case HFunction::Vector_String :
                            outfile << "\tvector<string>";
                            break;
                        case HFunction::Void :
                            outfile << "\tvector<string>";
                            break;
                    }
                    outfile << " " << var.first << " = ";
                    bool funcCall = false;
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
                            case Token::keyword:
                                outfile << tokenDat.getContents() << "(";
                                funcCall = true;
                                break;
                            case Token::function_call_continue:
                                break;
                            default:
                                outfile << tokenDat.getContents();
                                if (funcCall) {
                                    outfile << ")";
                                    funcCall = false;
                                }
                        }
                        current = current->left;
                        //outfile << current->data.getContents();
                    }
                    outfile << ";\n";
                }
        }
        else {
            int i = 0, j = 0;
            while(i < func.where_order.size() || j < func.commands.size()) {
               if (i < func.where_order.size() && j < func.commands.size()) {
                   if (func.where_order[i].second < func.commands[j].second) {
                       auto current = &get<0>(func.where[func.where_order[i].first]);
                       typing(get<1>(func.where[func.where_order[i].first]));
                       outfile << " " << func.where_order[i].first << " = ";
                       iterate_expression(current, p);
                       outfile << ";\n";
                       //process func.where_order[i]
                       i++;
                   } else {
                       //process func.commands[j]
                       auto current = &func.commands[j].first;
                       outfile << "\t";
                       iterate_expression(current, p);
                       outfile << ";\n";
                       j++;
                   }
               } else if (i < func.where_order.size()) {
                   //process func.where_order[i]
                   auto current = &get<0>(func.where[func.where_order[i].first]);
                   typing(get<1>(func.where[func.where_order[i].first]));
                   outfile << " " << func.where_order[i].first << " = ";
                   iterate_expression(current, p);
                   outfile << ";\n";
                   //process func.where_order[i]
                   i++;
               } else {
                   //process func.where_order[j]
                   auto current = &func.commands[j].first;
                   outfile << "\t";
                   iterate_expression(current, p);
                   outfile << ";\n";
                   j++;
               }
            }
        }
        if(func.name != "main") outfile << "}\n\n";
    }

    postamble();
    return 0;
}

void iterate_expression(HExpression* current, Parser p) {
    bool funcCall = false;
    int numParams = 0;
    stack<HExpression *> stk;
    while (current != nullptr || !stk.empty()) {
        while (current == nullptr && !stk.empty()) {
            current = stk.top()->right;
            stk.pop();
        }
        if (current == nullptr)
            break;
        stk.push(current);
        Token tokenDat = current->data;
        switch (tokenDat.getType()) {
            case Token::multiplicative_op:
                if (tokenDat.getContents() != "*" && tokenDat.getContents() != "/") {
                    if (tokenDat.getContents() == "`mod`")
                        outfile << " % ";
                    else if (tokenDat.getContents() == "`div`")
                        outfile << " / ";
                } else {
                    outfile << tokenDat.getContents();
                }if (numParams > 1) {
            outfile << ", ";
            numParams--;
        } else if (numParams == 1) {
            outfile << ")";
            numParams--;
        }
                break;
            case Token::v_empty: {
                outfile << "{}";
                if (numParams > 1) {
                    outfile << ", ";
                    numParams--;
                } else if (numParams == 1) {
                    outfile << ")";
                    numParams--;
                }
                break;
            }
            case Token::v_h_char:
            case Token::v_h_string:
            case Token::v_h_int:
            case Token::v_h_float:
                outfile << "{" << tokenDat.getContents().substr(1, tokenDat.getContents().length() - 2) << "}";
                if (numParams > 1) {
            outfile << ", ";
            numParams--;
        } else if (numParams == 1) {
            outfile << ")";
            numParams--;
        }
                break;
            case Token::keyword:
                outfile << tokenDat.getContents() << "(";
                funcCall = true;
                break;
            case Token::name: {
                auto it = find(p.function_table.begin(), p.function_table.end(), tokenDat.getContents());
                if (it != p.function_table.end()) {
                    HFunction f;
                    f.name = tokenDat.getContents();
                    auto it2 = find(p.functions_list.begin(), p.functions_list.end(), f);
                    numParams = it2->params_order.size();
                    outfile << tokenDat.getContents() << "(";
                } else {
                    outfile << tokenDat.getContents();
                    if (funcCall) {
                        outfile << ")";
                        funcCall = false;
                    } else if (numParams > 1) {
                        outfile << ", ";
                        numParams--;
                    } else if (numParams == 1) {
                        outfile << ")";
                        numParams--;
                    }
                }
                break;
            }
            case Token::get_line:
                outfile << "getLine(";
                funcCall = true;
                break;
            case Token::function_call_continue:
                break;
            default:
                outfile << tokenDat.getContents();
                if (funcCall) {
                    outfile << ")";
                    funcCall = false;
                } else if (numParams > 1) {
                    outfile << ", ";
                    numParams--;
                } else if (numParams == 1) {
                    outfile << ")";
                    numParams--;
                }
        }
        current = current->left;
        //outfile << current->data.getContents();
    }
}

    void typing(HFunction::Type type) {
        switch (type){
            case HFunction::Integer :
            outfile << "\tint";
            break;
            case HFunction::Float :
            outfile << "\tfloat";
            break;
            case HFunction::String :
            outfile << "\tstring";
            break;
            case HFunction::Char :
            outfile << "\tchar";
            break;
            case HFunction::Vector_Integer :
            outfile << "\tvector<int>";
            break;
            case HFunction::Vector_Float :
            outfile << "\tvector<float>";
            break;
            case HFunction::Vector_Char :
            outfile << "\tvector<char>";
            break;
            case HFunction::Vector_String :
            case HFunction::Void :
            outfile << "\tvector<string>";
            break;
}
}
