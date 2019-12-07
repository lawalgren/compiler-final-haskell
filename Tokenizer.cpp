//
// Created by lucas on 9/27/19.
//
#pragma once
#include <regex>
#include <vector>
#include <iostream>
#include "Token.cpp"

using namespace std;

class Tokenizer {
	vector<Token> tokens;
	vector<Token>::iterator it;

public:
	explicit Tokenizer(string line) {
		string token, remaining = std::move(line), error;
        bool fresh_newline = false;
		smatch sm;

		while (!remaining.empty()) {
		    if(remaining.substr(0,2) == "--") { // eat single line comments
		        while(!remaining.empty() && remaining[0] != '\n')
		            remaining = remaining.substr(1);
		    }
		    if(remaining.substr(0,2) == "{-") { // eat multiline comments
		        while(!remaining.empty() && remaining.substr(0,2) != "-}")
		            remaining = remaining.substr(1);
		    }
		    if(!tokens.empty() && !fresh_newline && remaining[0] == ' ') { // eat non-indenting spaces
		        while(remaining[0] == ' ')
		            remaining = remaining.substr(1);
		    }
            if (fresh_newline && remaining[0] != ' ') {
                tokens.emplace_back(Token(Token::end_of_function, ""));
                fresh_newline = false;
            }
		    else {
                Token t;
                if (regex_match(remaining, sm, regex("([^\n]+::.*)[\\S\\s]*"))) //match type decorators
                   t = Token(Token::type_decorator, sm[1]);
                else if (regex_match(remaining, sm, regex("(getLine|words|unlines|read|show|putStrLn|do|where|if|then|else)[\\S\\s]*")))
                    t = Token(Token::keyword, sm[1]);
                else if (regex_match(remaining, sm, regex("(Int|String|Char|Float|\\[Int\\]|\\[String\\]|\\[Char\\]|\\[Float\\])[\\S\\s]*")))
                    t = Token(Token::datatype, sm[1]);
                else if (regex_match(remaining, sm, regex("([A-Za-z\\_]+ *<- *getLine)[\\S\\s]*"))) {
                    string step1 = sm[1].str();
                    smatch sm1;
                    regex_match(step1, sm1, regex("([A-Za-z\\_]+)[\\S\\s]*"));
                    t = Token(Token::get_line, sm1[1]);
                }
                else if (regex_match(remaining, sm, regex("(\\(.*:.*\\))[\\S\\s]*")))
                    t = Token(Token::pattern, sm[1]);
                else if (regex_match(remaining, sm, regex("(\".*\")[\\S\\s]*")))
                    t = Token(Token::h_string, sm[1]);
                else if (regex_match(remaining, sm, regex("('.')[\\S\\s]*")))
                    t = Token(Token::h_char, sm[1]);
                else if (regex_match(remaining, sm, regex("([0-9]+\\.[0-9]+([Ee][+-]?[0-9]+)?)[\\S\\s]*")))
                    t = Token(Token::h_float, sm[1]);
                else if (regex_match(remaining, sm, regex("([0-9]+)[\\S\\s]*")))
                    t = Token(Token::h_int, sm[1]);
                else if (regex_match(remaining, sm, regex("(\\*|/|\\`mod\\`|\\`div\\`)[\\S\\s]*")))
                    t = Token(Token::multiplicative_op, sm[1]);
                else if (regex_match(remaining, sm, regex("(\\+\\+)[\\S\\s]*")))
                    t = Token(Token::plus_plus, sm[1]);
                else if (regex_match(remaining, sm, regex("([A-Za-z\\_]+)[\\S\\s]*")))
                    t = Token(Token::name, sm[1]);
                else if (regex_match(remaining, sm, regex("(\\[\\])[\\S\\s]*")))
                    t = Token(Token::v_empty, sm[1]);
                else if (regex_match(remaining, sm, regex("(\\[[A-Za-z\\_,\"]+\\])[\\S\\s]*")))
                    t = Token(Token::v_h_string, sm[1]);
                else if (regex_match(remaining, sm, regex("(\\[[A-Za-z_,']+\\])[\\S\\s]*")))
                    t = Token(Token::v_h_char, sm[1]);
                else if (regex_match(remaining, sm, regex("(\\[[0-9,]+\\])[\\S\\s]*")))
                    t = Token(Token::v_h_int, sm[1]);
                else if (regex_match(remaining, sm, regex("(\\[[0-9,\\.Ee\\+-]+\\])[\\S\\s]*")))
                    t = Token(Token::v_h_float, sm[1]);
                else if (regex_match(remaining, sm, regex("(\\+|-)[\\S\\s]*")))
                    t = Token(Token::plus_minus, sm[1]);
                else if (regex_match(remaining, sm, regex("(>=|<=|/=|==|>|<)[\\S\\s]*")))
                    t = Token(Token::conditional_op, sm[1]);
                else if (regex_match(remaining, sm, regex("(>=|<=|/=|==|>|<)[\\S\\s]*")))
                    t = Token(Token::conditional_op, sm[1]);
                else if (regex_match(remaining, sm, regex("(=)[\\S\\s]*")))
                    t = Token(Token::equals, sm[1]);
                else if (regex_match(remaining, sm, regex("(&&)[\\S\\s]*")))
                    t = Token(Token::logical_and, sm[1]);
                else if (regex_match(remaining, sm, regex("(\\|\\|)[\\S\\s]*")))
                    t = Token(Token::logical_or, sm[1]);
                else if (regex_match(remaining, sm, regex("(\\()[\\S\\s]*")))
                    t = Token(Token::open_paren, sm[1]);
                else if (regex_match(remaining, sm, regex("(\\))[\\S\\s]*")))
                    t = Token(Token::close_paren, sm[1]);
                else if (regex_match(remaining, sm, regex("( +)[\\S\\s]*")))
                    fresh_newline = false;
                else if (regex_match(remaining, sm, regex("([\r\n]+)[\\S\\s]*"))) {
                    //t = Token(Token::newline, sm[1]);
                    fresh_newline = true;
                }
                else if (regex_match(remaining, sm, regex("(\\|)[\\S\\s]*")))
                    t = Token(Token::vertical_bar, sm[1]);
                else if (regex_match(remaining, sm, regex("(:)[\\S\\s]*")))
                    t = Token(Token::colon, sm[1]);
                else {
                    error = "Did not recognize token: " + remaining;
                    tokens[0] = Token();
                    break;
                }
                remaining = remaining.substr(sm[1].length());
                if (t.getType() != Token::none)
                    tokens.emplace_back(t);

            }
		}
		if (error.empty()) it = tokens.begin();

		else {
			it = tokens.end();
			cerr << error << endl;
		}
	}

	Token peek() {
		if (it < tokens.end()) return (*it);
		else return Token(Token::eof);
	}

	Token next() {
		Token t = peek();
		if (t.getType() != Token::eof) it++;
		return t;
	}
};
