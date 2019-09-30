//
// Created by lucas on 9/27/19.
//
#pragma once
#include <string>
#include <unordered_set>
#include <utility>

using namespace std;

class Token {
    public:
        typedef enum _TokenTypes {
            special,
            whitechar,
            newline,
            dashes,
            opencom,
            closecom,
            ascSmall,
            // uniSmall,
            ascLarge,
            // uniLarge,
            ascSymbol,
            // uniSymbol,
            ascDigit,
            // uniDigit,
            reservedop,
            exponent,
            charesc,
            none,
            eof,
            // ascii
        } Type;

    private:
        unordered_set<Type> types;
        string contents;;

    public:
        Token() {
           types = {none};
           contents = "";
        }

        bool contains(Type type) { return(types.find(type) != types.end()); }

        void insertType(Type type) {
            if(contains(none)) types.clear();
            types.emplace(type);
        }

        string getContents() { return contents; }
        void setContents(string _contents) { contents = std::move(_contents); }
};
