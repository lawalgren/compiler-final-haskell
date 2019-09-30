//
// Created by lucas on 9/27/19.
//
#pragma once
#include <regex>
#include <vector>
#include <unordered_set>
#include <iostream>
#include "Token.cpp"

using namespace std;

class Tokenizer {
    vector<Token> tokens;
    vector<Token>::iterator it;

    explicit Tokenizer(string line) {
        string token, remaining = std::move(line), error;
        bool matched = false;
        smatch sm;

        while (!remaining.empty()) {
           Token t = Token();
           if(regex_match(remaining,sm, regex(R"((\(|\)|,|\[|\]|\`|\{|\}).*)"))) {
               t.insertType(Token::special);
               matched = true;
           }
           if(regex_match(remaining,sm, regex(R"((\r\n)|\r|\n|\f).*)"))) {
               t.insertType(Token::newline);
               matched = true;
           }
           if(regex_match(remaining,sm, regex(R"((---*).*)"))) {
               t.insertType(Token::dashes);
               matched = true;
           }
           if(regex_match(remaining,sm, regex(R"((\v| |\t).*)")) || t.contains(Token::newline)) {
               t.insertType(Token::whitechar);
               matched = true;
           }
           if(regex_match(remaining,sm, regex(R"((\{-).*)"))) {
                t.insertType(Token::opencom);
                matched = true;
           }
           if(regex_match(remaining,sm, regex(R"((-\}).*)"))) {
               t.insertType(Token::closecom);
               matched = true;
           }
           if(regex_match(remaining,sm, regex(R"(([a-z]+).*)"))) {
               t.insertType(Token::ascSmall);
               matched = true;
           }
           if(regex_match(remaining,sm, regex(R"(([A-Z]+).*)"))) {
               t.insertType(Token::ascLarge);
               matched = true;
           }
           //TODO: complete type matching

           if(!matched) {
               error = "Did not recognize token: " + remaining;
               tokens[0] = Token();
               break;
           }

           remaining = remaining.substr(sm[1].length());
        }
        if(error.empty()) it = tokens.begin();

        else {
            it = tokens.end();
            cerr << error << endl;
        }
    }

    Token peek() {
        if (it < tokens.end()) return (*it);
        else {
            Token t;
            t.insertType(Token::eof);
            return t;
        }
    }

    Token next() {
        Token t = peek();
        if (!t.contains(Token::eof)) it++;
        return t;
    }
};
