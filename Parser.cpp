//
// Created by lucas on 11/19/19.
//
#pragma once
#include "Tokenizer.cpp"
#include "HFunction.cpp"
#include <string>
#include <utility>
#include <vector>

class Parser {
    map<string,HFunction> functions_list;
    Tokenizer t;
public:
    explicit Parser(string program) : t(std::move(program)) {
        bool in_function = false;
        bool equals = false;
        bool iff = false;
        functions_list = {};
        HFunction curr_func = {};
        auto tokens = t.getTokens();
        for(auto next:tokens) {
            if(!in_function && next.getType() == Token::name) {
                if(!curr_func.name.empty())
                    functions_list[curr_func.name] = curr_func;
                curr_func = {};
                curr_func.name = next.getContents();
                in_function = true;
            }
            else if(in_function && next.getType() == Token::name && !equals) {
                curr_func.params.emplace_back(next.getContents());
            }
            else if(next.getType() == Token::equals) {
                equals = true;
            }
            else if(next.getContents() == "if") {

            }
        }
    }
};
