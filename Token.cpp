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
            h_string,
            h_int,
            h_float,
            newline,
            whitechar,
            eof,
            none
        } Type;

    private:
        Type type;
        string contents;;

    public:
        explicit Token(Type _type=none, string _contents="") {
           type = _type;
           contents = std::move(_contents);
        }

        Type getType() { return type; }
        string getContents() { return contents; }
};
