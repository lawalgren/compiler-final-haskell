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
		    if(tokens[-1].getType() != Token::newline && remaining[0] == ' ') { // eat non-indenting spaces
		        while(remaining[0] == ' ')
		            remaining = remaining.substr(1);
		    }
		    else {
                Token t;
                if (regex_match(remaining, sm, regex(R"(([^\n]+::.*).*)"))) //match type decorators
                   t = Token(Token::type_decorator, sm[1]);
                else if (regex_match(remaining, sm, regex(R"((getLine|words|unlines|read|show|putStrLn|do|where|if|then|else).*)")))
                    t = Token(Token::keyword, sm[1]);
                else if (regex_match(remaining, sm, regex(R"((Int|String|Char|Float|\[Int\]|\[String\]|\[Char\]|\[Float\]).*)")))
                    t = Token(Token::datatype, sm[1]);
                else if (regex_match(remaining, sm, regex(R"((\(.*:.*\)).*)")))
                    t = Token(Token::pattern, sm[1]);
                else if (regex_match(remaining, sm, regex(R"((".*").*)")))
                    t = Token(Token::h_string, sm[1]);
                else if (regex_match(remaining, sm, regex(R"(('.').*)")))
                    t = Token(Token::h_char, sm[1]);
                else if (regex_match(remaining, sm, regex(R"(([0-9]+\.[0-9]+([Ee][+-]?[0-9]+)?).*)")))
                    t = Token(Token::h_float, sm[1]);
                else if (regex_match(remaining, sm, regex(R"(([0-9]+).*)")))
                    t = Token(Token::h_int, sm[1]);
                else if (regex_match(remaining, sm, regex(R"((\*|/|\`mod\`|\`div\`).*)")))
                    t = Token(Token::multiplicative_op, sm[1]);
                else if (regex_match(remaining, sm, regex(R"((\+\+).*)")))
                    t = Token(Token::plus_plus, sm[1]);
                else if (regex_match(remaining, sm, regex(R"(([A-Za-z_]+).*)")))
                    t = Token(Token::name, sm[1]);
                else if (regex_match(remaining, sm, regex(R"((\+|-).*)")))
                    t = Token(Token::plus_minus, sm[1]);
                else if (regex_match(remaining, sm, regex(R"((>=|<=|/=|==|>|<).*)")))
                    t = Token(Token::conditional_op, sm[1]);
                else if (regex_match(remaining, sm, regex(R"((>=|<=|/=|==|>|<).*)")))
                    t = Token(Token::conditional_op, sm[1]);
                else if (regex_match(remaining, sm, regex(R"((=).*)")))
                    t = Token(Token::equals, sm[1]);
                else if (regex_match(remaining, sm, regex(R"((&&).*)")))
                    t = Token(Token::logical_and, sm[1]);
                else if (regex_match(remaining, sm, regex(R"((\|\|).*)")))
                    t = Token(Token::logical_or, sm[1]);
                else if (regex_match(remaining, sm, regex(R"((\().*)")))
                    t = Token(Token::open_paren, sm[1]);
                else if (regex_match(remaining, sm, regex(R"((\)).*)")))
                    t = Token(Token::close_paren, sm[1]);
                else if (regex_match(remaining, sm, regex(R"(( +).*)")))
                    t = Token(Token::whitechar, sm[1]);
                else if (regex_match(remaining, sm, regex(R"(([\r\n]+).*)"))) {
                    t = Token(Token::newline, sm[1]);
                    fresh_newline = true;
                }
                else if (regex_match(remaining, sm, regex(R"((\|).*)")))
                    t = Token(Token::vertical_bar, sm[1]);
                else if (regex_match(remaining, sm, regex(R"((:).*)")))
                    t = Token(Token::colon, sm[1]);
                else {
                    error = "Did not recognize token: " + remaining;
                    tokens[0] = Token();
                    break;
                }
                remaining = remaining.substr(sm[1].length());
                tokens.emplace_back(t);
                if (fresh_newline && t.getType() == Token::whitechar)
                    fresh_newline = false;
                else if (fresh_newline) {
                    tokens.emplace_back(Token(Token::end_of_function, ""));
                    fresh_newline = false;
                }
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
