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
#include <sstream>
#include <stack>

using namespace std;

class Parser {
public:
    vector<HFunction> functions_list;
    vector<string> function_table;
    Tokenizer t;
    string error = "";
    explicit Parser(string program) : t(move(program)) {
        while(t.peek().getType() != Token::eof)
           functions_list.emplace_back(function());
        if(!error.empty())
            cout << error << endl;
    }
    // <functionName> <params> = <result> <where> | <functionName> = do <commands>
    // | <functionName> <params> <guards> <where>
    HFunction function() {
       HFunction func;
       func.purity = true;

       // get function param types and return type
        vector<string> param_returns = {};
        if(t.peek().getType() == Token::type_decorator) {
           string type = t.next().getContents();
           istringstream ss(type);
           string temp;
           vector<string> tempvec;
           while (ss >> temp) {
               if (temp != "->" && temp != "::")
                   tempvec.emplace_back(temp);
           }
           t.next(); // remove eof token after type decorator
           string return_type = tempvec[tempvec.size() - 1];
           func.return_type = strToType(return_type);
           for (long i = 1; i < tempvec.size() - 1; i++) {
               param_returns.emplace_back(tempvec[i]);
           }
       }

       // get function name
       Token nam = t.next();
       func.name = nam.getContents();

       // get function parameters and map them to their type
        vector<string> param_vector = params();
        map<string, HFunction::Type> param_map;
        if (find(function_table.begin(), function_table.end(), func.name) != function_table.end()) {
            auto it = find(functions_list.begin(), functions_list.end(), func);
            unsigned long i = 0;
            for (auto & param : it->params_order) {
                //TODO: Test this
                param_map[param_vector[i]] = it->params[param];
                func.params_order.emplace_back(param_vector[i]);
                i++;
            }
            func.return_type = it->return_type;
        } else {
            for (unsigned long i = 0; i < param_vector.size(); i++) {
                string _type = param_returns[i];
                HFunction::Type h_type = strToType(_type);
                param_map[param_vector[i]] = h_type;
                func.params_order.emplace_back(param_vector[i]);
            }
        }
        func.params = param_map;

        if (t.peek().getType() == Token::vertical_bar) {
           func.logic = {};
           guards(func.logic, func);
           func.where = {};
           where(func.where, func);
       } else if (t.peek().getContents() == "do") {
           t.next();
           func.purity = false;
           auto a = commands(func);
           func.commands = get<0>(a);
           func.where = get<1>(a);
       } else {
           t.next();
           func.logic = {};
           result(func.logic, func);
           func.where = {};
           where(func.where, func);
       }
       function_table.emplace_back(func.name);
       if(t.peek().getType() == Token::end_of_function)
            t.next();
       return func;
    }

    static HFunction::Type strToType(const string& _type) {
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
        return h_type;
    }

    void guards(map<HLogical, tuple<HExpression, HFunction::Type>> &logic, HFunction &func) { // | <logical> = <expresssion>
        if (t.peek().getType() != Token::vertical_bar)
            return;
        t.next();
        HLogical hl;
        logical(hl);
        t.next(); //eat = sign
        pair<HExpression, HFunction::Type> exp = expression();
        pair<HLogical, HExpression> key = {hl, exp.first};
        logic[hl] = make_tuple(exp.first, exp.second);
        func.logic_order.emplace_back(hl);
        guards(logic, func);
    }

    void where(map<string, tuple<HExpression, HFunction::Type>> &vars, HFunction &func) { // where <assignment>
        if (t.peek().getContents() == "where")
            t.next();
        if (t.peek().getType() != Token::name)
            return;
        auto a = assignment();
        vars[get<0>(a)] = make_tuple(get<1>(a), get<2>(a));
        func.where_order.emplace_back(get<0>(a));
        where(vars, func);
    }

    tuple<string, HExpression, HFunction::Type> assignment() { // <variable> = <expression>
        string name = t.next().getContents();
        t.next(); //eat = sign
        pair<HExpression, HFunction::Type> exp = expression();
        return make_tuple(name, exp.first, exp.second);
    }

     void result(map<HLogical, tuple<HExpression, HFunction::Type>> &logic, HFunction &func) { // <conditionalExp> | <expression>
        if(t.peek().getContents() == "if") {
            conditionalExp(logic);
        } else {
            auto exp = expression();
            pair<HLogical, HExpression> key = {HLogical(), exp.first};
            logic[HLogical()] = make_tuple(exp.first, exp.second);
            func.logic_order.emplace_back(HLogical());
        }
    }

    void conditionalExp(map<HLogical, tuple<HExpression, HFunction::Type>> &logic) { // if <logical> then <expression> else <expression>
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
        logic[hc] = make_tuple(then.first,then.second);
        logic[!hc] = make_tuple(_else.first, _else.second);
    }

    void logical(HLogical &hl) { // <conditional> <logical_op> <conditional>
        auto *hc = new HCondition();
        auto *hc2 = new HCondition();
        conditional(hc);
        string op = t.next().getContents();
        conditional(hc2);
        hl.left = hc;
        hl.right = hc2;
        hl.op = op;
    }

    void conditional(HCondition* &hc) { // <expression> <conditional_op> <expression>
        auto exp1 = expression();
        string op = t.next().getContents();
        auto exp2 = expression();
        hc->left = &exp1.first;
        hc->right = &exp2.first;
        hc->op = op;
    }

    pair<HExpression, HFunction::Type> expression() { //gets an abstract syntax tree for each each expression
        auto *he = new HExpression();
        expressionHelper(he);
        pair<HExpression, HFunction::Type> ret = {*he,getType(he)};
        return ret;
    }

    static HFunction::Type test(Token tok, bool &read, bool &show, bool &vector) { //checks for constants to determine expression type
        if(tok.getType() == Token::v_h_string)
            return HFunction::Vector_String;
        else if(tok.getType() == Token::v_h_char)
            return HFunction::Vector_Char;
        else if(tok.getType() == Token::v_h_int)
            return HFunction::Vector_Integer;
        else if(tok.getType() == Token::v_h_float)
            return HFunction::Vector_Float;
        else if(tok.getType() == Token::h_string)
            if (read&&vector)
                return HFunction::Vector_Integer;
            else if (read)
                return HFunction::Integer;
            else if (vector)
                return HFunction::Vector_String;
            else
                return HFunction::String;
        else if(tok.getType() == Token::h_char)
            if (read&&vector)
                return HFunction::Vector_Integer;
            else if (read)
                return HFunction::Integer;
            else if (vector)
                return HFunction::Vector_Char;
            else
                return HFunction::Char;
        else if(tok.getType() == Token::h_int)
            if (vector&&show)
                return HFunction::Vector_String;
            else if (show)
                return HFunction::String;
            else if (vector)
                return HFunction::Vector_Integer;
            else
                return HFunction::Integer;
        else if(tok.getType() == Token::h_float)
            if (show && vector)
                return HFunction::Vector_String;
            else if (show)
                return HFunction::String;
            else if (vector)
               return HFunction::Vector_Float;
            else
                return HFunction::Float;
        else {
            if(tok.getType() == Token::colon)
                vector = true;
            else if(tok.getType() == Token::plus_plus)
                return HFunction::String;
            else if(tok.getContents() == "read")
                read = true;
            else if(tok.getContents() == "show")
                show = true;
        }
        return HFunction::Void;
    }

    static HFunction::Type getType(HExpression* he) { // iterates over expression tree and attempts to determine type
        auto current = he;
        HFunction::Type ty = HFunction::Void;
        HFunction::Type ty2;
        stack<HExpression*> st;
        bool read = false, show = false, vector = false;
        while(current != nullptr || !st.empty()) {
            while (current == nullptr && !st.empty()) {
                current = st.top()->right;
                st.pop();
            }
            if (current == nullptr)
                break;
            st.push(current);
            ty2 = test(current->data, read, show, vector);
            if (ty2 != HFunction::Void)
                ty = ty2;
            current = current->left;
        }
        return ty;
    }

    bool expressionHelper(HExpression* &he) { // <expression> <additive_op> <term>
        auto* sub = new HExpression();
        HExpression* left = nullptr;
        Token last, next;
        while (term(sub)) {
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

    bool term(HExpression* &he) { // <term> <multiplicative_op> <unary_expression>
        auto* sub = new HExpression();
        HExpression* left = nullptr;
        Token last, next;
        while (unaryExpression(sub)) {
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

    bool unaryExpression(HExpression* &he) { // <plus_minus> <unary_expression> | <primary_expression>
        Token operation = t.peek();
        if (operation.getType() == Token::plus_minus) {
            auto* sub = new HExpression();
            operation = t.next();
            if (unaryExpression(sub)) {
                he = new HExpression(operation, nullptr, sub);
                return true;
            }
        } else return (primaryExpression(he));
        return false;
    }

    bool primaryExpression(HExpression* &he) { // <functionCall> | <variable> | (<expression>) | <constant>
       Token primary = t.peek();
       if (primary.getType() == Token::name) {
           if (find(function_table.begin(), function_table.end(), primary.getContents()) != function_table.end())
               return functionCall(he);
           else {
               he = new HExpression(t.next()); // the next token is a variable name
               return true;
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
                return true;
            }
       }
    }

    bool paramTree(HExpression* &he) { // <param> <paramTree> | <epsilon>
        auto *sub = new HExpression();
        HExpression *left  = nullptr;
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

    vector<string> params() { // <param> <params>
        Token token = t.peek();
        vector<string> param_list;
        while(token.getType() != Token::equals && token.getType() != Token::vertical_bar) {
            t.next();
            param_list.emplace_back(token.getContents());
            token = t.peek();
        }
        return param_list;
    }

    bool functionCall(HExpression* &he) { // <functionName> <paramTree>
        Token name = t.next();
        he = new HExpression(name);
        auto* sub = new HExpression();
        paramTree(sub);
        he = new HExpression(name, nullptr, sub);
        return true;
    }

    // let <assignment> <commands> | <variable> <- getLine <commands> | <functionCall> <commands> | <epsilon>
    pair<vector<HExpression>,map<string, tuple<HExpression, HFunction::Type>>> commands(HFunction &func) {
        Token tok = t.peek();
        vector<HExpression> command_list;
        map<string, tuple<HExpression, HFunction::Type>> lets;
        while(tok.getType() != Token::end_of_function) {
            tok = t.next();
            if(tok.getContents() == "let") {
                auto a = assignment();
                lets[get<0>(a)] = make_tuple(get<1>(a), get<2>(a));
                func.where_order.emplace_back(get<0>(a));
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
        pair<vector<HExpression>,map<string, tuple<HExpression, HFunction::Type>>> ret = {command_list, lets};
        return ret;
    }
};
