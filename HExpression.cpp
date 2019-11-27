//
// Created by lucas on 11/20/19.
//

#include <string>
#include <vector>

using namespace std;

struct Node {
    int idata;
    string sdata;
    char cdata;
    float fdata;
    vector<int> vidata;
    vector<string> vsdata;
    vector<char> vcdata;
    vector<float> vfdata;
};

struct HExpression {
    HExpression* left;
    Node data;
    string op;
    HExpression* right;
};
