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
#include <algorithm>

using namespace std;

class Parser {
    vector<HFunction> functions_list;
    vector<string> function_table;
    Tokenizer t;
    string error = "";
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
       function_table.emplace_back(func.name);

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
           func.logic = {};
           guards(func.logic);
           func.where = {};
           where(func.where);
       }
       else if (t.peek().getContents() == "do") {
           t.next();
           func.purity = false;
           auto a = commands();
           func.commands = a.first;
           func.where = a.second;
       }
       else {
           func.logic = {};
           result(func.logic);
           func.where = {};
           where(func.where);
       }
    }

    void guards(map<pair<HLogical, HExpression>, HFunction::Type> &logic) {
        if (t.peek().getType() != Token::vertical_bar)
            return;
        t.next();
        HLogical hl;
        logical(hl);
        t.next();
        pair<HExpression, HFunction::Type> exp = expression();
        pair<HLogical, HExpression> key = {hl, exp.first};
        logic[key] = exp.second;
        guards(logic);
    }

    void where(map<pair<string, HExpression>, HFunction::Type> &vars) {
        if (t.peek().getContents() == "where")
            t.next();
        if (t.peek().getType() != Token::name)
            return;
        auto a = assignment();
        vars[a.first] = a.second;
        where(vars);
    }

    pair<pair<string, HExpression>, HFunction::Type> assignment() {
        string name = t.next().getContents();
        t.next();
        pair<HExpression, HFunction::Type> exp = expression();
        pair<string, HExpression> key = {name, exp.first};
        return {key, exp.second};
    }

     void result(map<pair<HLogical, HExpression>, HFunction::Type> &logic) {
        if(t.peek().getContents() == "if")
            conditionalExp(logic);
        else {
            auto exp = expression();
            pair<HLogical, HExpression> key = {HLogical(), exp.first};
            logic[key] = exp.second;
        }
    }

    void conditionalExp(map<pair<HLogical, HExpression>, HFunction::Type> &logic) {
        if(t.peek().getContents() != "if") {
            cout << "error";
            return;
        }
        t.next(); // remove "if" token
        HLogical hc;
        logical(hc);
        t.next(); // remove "then" token
        auto then = expression();
        t.next(); // remove "else" token
        auto _else = expression();
        pair<HLogical, HExpression> key1 = {hc, then.first};
        pair<HLogical, HExpression> key2 = {!hc, _else.first};
        logic[key1] = then.second;
        logic[key2] = _else.second;
    }

    void logical(HLogical &hl) {
        auto *hc = new HCondition();
        auto *hc2 = new HCondition();
        conditional(hc);
        string op = t.next().getContents();
        conditional(hc2);
        hl.left = hc;
        hl.right = hc2;
        hl.op = op;
    }

    void conditional(HCondition* &hc) {
        auto exp1 = expression();
        string op = t.next().getContents();
        auto exp2 = expression();
        hc->left = &exp1.first;
        hc->right = &exp2.first;
        hc->op = op;
    }

    pair<HExpression, HFunction::Type> expression() {
        auto *he = new HExpression();
        expressionHelper(he);
        pair<HExpression, HFunction::Type> ret = {*he,getType(he)};
        return ret;
    }

    HFunction::Type getType(HExpression* he) {

    } // TODO: Extract type from an arbitrary expression

    bool expressionHelper(HExpression* &he) {
        Token t1 = t.next();
        auto* sub = new HExpression();
        HExpression* left = nullptr;
        Token last, next;
        while (term(he)) {
            if (t.peek().getType() != Token::plus_minus || t.peek().getType() != Token::plus_plus || t.peek().getType() != Token::colon)
                break;
            else {
                next = t.next();
                if (left != nullptr)
                    left = new HExpression(last,left,sub);
                else
                    left = new HExpression(sub);
            }
            sub = new HExpression();
            last = next;
        }
        if (left != nullptr)
            he = new HExpression(last, left, sub);
        else
            he = new HExpression(sub);
        return true;
    }

    bool term(HExpression* &he) {
        Token t1 = t.next();
        auto* sub = new HExpression();
        HExpression* left = nullptr;
        Token last, next;
        while (unaryExpression(he)) {
            if (t.peek().getType() != Token::multiplicative_op)
                break;
            else {
                next = t.next();
                if (left != nullptr)
                    left = new HExpression(last,left,sub);
                else
                    left = new HExpression(sub);
            }
            sub = new HExpression();
            last = next;
        }
        if (left != nullptr)
            he = new HExpression(last, left, sub);
        else
            he = new HExpression(sub);
        return true;
    }

    bool unaryExpression(HExpression* &he) {
        Token operation = t.peek();
        if (operation.getType() == Token::plus_minus) {
            auto* sub = new HExpression();
            operation = t.next();
            if (unaryExpression(sub)) {
                he = new HExpression(operation, nullptr, sub);
                return true;
            }
        } else return (primaryExpression(he));
    }

    bool primaryExpression(HExpression* &he) {
       Token primary = t.peek();
       if (primary.getType() == Token::name) {
           if (find(function_table.begin(), function_table.end(), primary.getContents()) != function_table.end())
               return functionCall(he);
           else {
               he = new HExpression(t.next()); // the next token is a variable name
           }
       } else {
            if (primary.getType() == Token::open_paren) {
                t.next(); // eliminate open paren
                auto* sub = new HExpression();
                if (expressionHelper(he)) {
                    he = new HExpression(sub);
                    if (t.next().getType() != Token::close_paren) {
                        error = "Error: Mismatched (";
                        return false;
                    } else return true;
                } else {
                    error = "Error: Expected expression within ()";
                    return false;
                }
            } else {
                he = new HExpression(t.next()); // the next token is a constant
            }
       }
    }

    bool paramTree(HExpression* &he) {
        auto *sub = new HExpression();
        HExpression *left = nullptr;
        while (primaryExpression(sub)) {
            if (t.peek().getType() == Token::end_of_function || t.peek().getType() == Token::close_paren)
                return false;
            if (left != nullptr)
                left = new HExpression(Token(Token::function_call_continue), left, sub);
            else
                left = new HExpression(sub);
        }
        if (left != nullptr)
            he = new HExpression(Token(Token::function_call_continue), left, sub);
        else
            he = new HExpression(sub);
        return true;
    }

    vector<string> params() {
        Token token = t.peek();
        vector<string> param_list;
        while(token.getContents() != "=" && token.getContents() != "|")
            param_list.emplace_back(t.next());
        return param_list;
    }

    bool functionCall(HExpression* &he) {
        Token name = t.next();
        he = new HExpression(name);
        auto* sub = new HExpression();
        paramTree(sub);
        he = new HExpression(name, nullptr, sub);
        return true;
    }

    pair<vector<HExpression>,map<pair<string, HExpression>, HFunction::Type>> commands() {
        Token tok = t.peek();
        vector<HExpression> command_list;
        map<pair<string, HExpression>, HFunction::Type> lets;
        while(tok.getType() != Token::end_of_function) {
            tok = t.next();
            if(tok.getContents() == "let") {
                auto a = assignment();
                lets[a.first] = a.second;
            } else if (tok.getType() == Token::get_line) {
                auto* nested = new HExpression(Token(Token::name, tok.getContents()));
                HExpression he(Token(Token::get_line), nullptr, nested);
                command_list.emplace_back(he);
            } else {
                auto* he = new HExpression();
                functionCall(he);
                command_list.emplace_back(*he);
            }
        }
        pair<vector<HExpression>,map<pair<string, HExpression>, HFunction::Type>> ret = {command_list, lets};
        return ret;
    }
};
