//
// Created by lucas on 11/19/19.
//
#pragma once
#include <map>
#include <vector>
#include <string>
#include "HCondition.cpp"
#include "HLogical.cpp"

using namespace std;

struct HFunction{
    typedef enum _Type{
        Integer, Float, String, Char, Vector_Integer, Vector_Float, Vector_Char, Vector_String, Void
    } Type;
    string name;
    map<HLogical, tuple<HExpression, Type>> logic; // map condition to returned value
    map<string, tuple<HExpression, Type>> where; // map variable name to value
    map<string, Type> params; // list of parameters to the function
    vector<HLogical> logic_order;
    vector<pair<string, int>> where_order;
    vector<string> params_order;
    bool purity;
    Type return_type;
    vector<pair<HExpression, int>> commands;

};

inline bool operator==(const HFunction &h1, const HFunction &h2) {
    return(h1.name == h2.name);
}
