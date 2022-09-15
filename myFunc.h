//
// Created by xkzmx on 12/13/2021.
//

#ifndef CIRCUITS_MYFUNC_H
#define CIRCUITS_MYFUNC_H
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

class circuit{
public:
    int inputs; // number of inputs
    int outputs; // number of outputs
    int rowNum{};
    int simplifiedRowNum{};
    vector<int> feature;
    vector<vector<int>*> truthTable;
    circuit();
    circuit(int inputs, int outputs);
    circuit(int inputs, int outputs,  vector<vector<int>*> truthTable);
    ~circuit();
    void printCircuit()const;
    void printCanonicalForm()const;
    void setFeatureOfCircuit();
private:
    vector<vector<int>*> simplifiedTable;
    vector<vector<int>*> tempForm;

    vector<vector<int>*> canonicalForm;

    void simplify();
    void standardize();
    vector<pair<int,int>> getRowSum()const;
    vector<pair<int,int>> getColSum()const;
    vector<int> finRowSum()const;
    vector<int> finColSum()const;
    void swapRow(const vector<pair<int,int>>&sum);
    void swapCol(const vector<pair<int,int>>&sum);
    void printSimplified()const;

    void printTempForm()const;

};

class myHashEqualClass {

public:
    bool operator()(const vector<int> &c1, const vector<int> &c2) const;

    size_t operator()(const vector<int> &c) const;
};
ostream &operator<<(ostream &str, const unordered_map<vector<int>, circuit,myHashEqualClass,myHashEqualClass>& DB);
bool findCircuit(int input, int output, vector<vector<int> *>&c, unordered_map<vector<int>, circuit,myHashEqualClass,myHashEqualClass>& DB);
#endif //CIRCUITS_MYFUNC_H
