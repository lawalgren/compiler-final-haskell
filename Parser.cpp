//
// Created by lucas on 11/19/19.
//
#pragma once
#include "Tokenizer.cpp"
#include "HFunction.cpp"
#include <string>
#include <utility>
#include <vector>
#include <regex>

using namespace std;

class Parser {
    vector<HFunction> functions_list;
    Tokenizer t;
public:
    explicit Parser(string program) : t(move(program)) {

    }

    void function() {
       HFunction func;
       func.purity = true;

       // get function param types and return type
       string type = t.next().getContents();
       smatch sm;
       regex_match(type, sm, regex(R"(^[ ] \:\: ([A-Za-z\[\]]+->)+([A-Za-z\[\]))"));
       string return_type = sm[sm.length() - 1];
       vector <string> param_returns = {};
       for (long i = 1; i < sm.length() - 1; i++) {
          param_returns.emplace_back(sm[i]);
       }

       // get function name
       func.name = t.next().getContents();

       // get function parameters and map them to their type
       vector<string> param_vector = params();
       map<string, HFunction::Type> param_map;
       for (unsigned long i = 0; i < param_vector.size(); i++) {
           string _type = param_returns[i];
           HFunction::Type h_type;

           if (_type == "Int")
               h_type = HFunction::Integer;
           else if (_type == "String")
               h_type = HFunction::String;
           else if (_type == "Char")
               h_type = HFunction::Char;
           else if (_type == "Float")
               h_type = HFunction::Float;
           else if (_type == "[Int]")
               h_type = HFunction::Vector_Integer;
           else if (_type == "[String]")
               h_type = HFunction::Vector_String;
           else if (_type == "[Char]")
               h_type = HFunction::Vector_Char;
           else if (_type == "[Float]")
               h_type = HFunction::Vector_Float;
           else
               h_type = HFunction::Void;

          param_map[param_vector[i]] = h_type;
       }
       func.params = param_map;

       if (t.peek().getType() == Token::vertical_bar) {
           func.logic = guards();
           func.where = where();
       }
       else if (t.peek().getContents() == "do") {
           t.next();
           func.purity = false;
           func.commands = commands();
       }
       else {
           func.logic = result();
           func.where = where();
       }
    }

    map<HCondition, string, HFunction::Type> guards() {
        map<HCondition, string, HFunction::Type> logic = {};
        if (t.peek().getType() != Token::vertical_bar)
            return logic;

        HCondition hc = logical();
        t.next();
        pair<string, HFunction::Type> exp = expression();
        logic[hc][exp.first] = exp.second;

    }

    map<string, string, HFunction::Type> where() {

    }

    void assignment() {

    }

    map<HCondition, string, HFunction::Type> result() {

    }

    void conditionalExp() {

    }

    HCondition logical() {

    }

    void conditional() {

    }

    void expression() {

    }

    void term() {

    }

    void factor() {

    }

    vector<string> params() {

    }

    void functionCall() {

    }

    vector<string> commands() {

    }

    void parameter() {

    }

    void constant() {

    }

    void vaiable() {

    }
};
