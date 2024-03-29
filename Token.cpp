//
// Created by lucas on 9/27/19.
//
#pragma once
#include <string>
#include <utility>

using namespace std;

class Token {
    public:
        typedef enum _TokenTypes {
            keyword,
            datatype,
            name,
            get_line,
            type_decorator,
            multiplicative_op,
            plus_minus,
            conditional_op,
            pattern,
            logical_and,
            logical_or,
            vertical_bar,
            colon,
            plus_plus,
            equals,
            open_paren,
            close_paren,
            h_char,
            v_empty,
            v_h_char,
            h_string,
            v_h_string,
            h_int,
            v_h_int,
            h_float,
            v_h_float,
            newline,
            whitechar,
            eof,
            end_of_function,
            function_call_continue,
            none
        } Type;

    private:
        Type _type;
        string contents;;

    public:
        explicit Token(Type __type=none, string _contents="") {
           _type = __type;
           contents = std::move(_contents);
        }

        Type getType() { return _type; }
        string getContents() { return contents; }
};
