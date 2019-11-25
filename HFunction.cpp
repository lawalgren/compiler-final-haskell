//
// Created by lucas on 11/19/19.
//
#pragma once
#include <map>
#include <vector>
#include <string>
#include "HCondition.cpp"

using namespace std;

/*
struct ReturnValue {
    int iret;
    string sret;
    char cret;
    float fret;
    vector<int> viret;
    vector<char> ciret;
    vector<string> siret;
    vector<float> firet;
    ReturnType return_type;
};
*/

struct HFunction{
    typedef enum _Type{
        Integer, Float, String, Char, Vector_Integer, Vector_Float, Vector_Char, Vector_String, Void
    } Type;
    string name;
    map<HCondition,string, Type> logic; // map condition to returned value
    map<string,string, Type> where; // map variable name to value
    map<string, Type> params; // list of parameters to the function
    bool purity;
    Type return_type;
    vector<string> commands;
};
