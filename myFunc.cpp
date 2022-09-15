//
// Created by xkzmx on 12/13/2021.
//

#include "myFunc.h"

circuit::circuit() {
    inputs = 0;
    outputs = 0;

}

void circuit::simplify() {
    ///  remove rows that has output 0

    for (auto &table: truthTable) {
        int sum = 0;
        for (int i = 0; i < outputs; i++) {
            sum += table->at(i + inputs);
        }
        if (sum != 0) {
            auto temp = new vector<int>;
            for (const auto &item: *table) {
                temp->push_back(item);
            }
            simplifiedTable.push_back(temp);
            simplifiedRowNum++;
        }
    }

}

circuit::circuit(int _inputs, int _outputs) {
    inputs = _inputs;
    outputs = _outputs;
    rowNum = 1 << inputs;
}
circuit::circuit(int _inputs, int _outputs, vector<vector<int> *> _truthTable) {
    inputs = _inputs;
    outputs = _outputs;
    rowNum = 1 << inputs;
    for (const auto &item: _truthTable) {
        auto temp = new vector<int>;
        for (const auto &item2: *item) {
            temp->push_back(item2);
        }
        truthTable.push_back(temp);
    }
    setFeatureOfCircuit();

}
circuit::~circuit() {
//        for (auto& i: truthTable) {
//            delete i;
//        }
//        truthTable.clear();
//        for (auto &i: simplifiedTable) {
//            delete i;
//        }
//        simplifiedTable.clear();
//        for (auto &i: canonicalForm) {
//            delete i;
//        }
//    canonicalForm.clear();
//        for (auto &i: tempForm) {
//            delete i;
//        }
//    tempForm.clear();

    inputs = 0;
    outputs = 0;
    rowNum = 0;
    simplifiedRowNum = 0;
}

void circuit::setFeatureOfCircuit() {
    standardize();
    feature.push_back(inputs);
    feature.push_back(outputs);
    feature.push_back(simplifiedRowNum);

    auto rowSum = finRowSum();
    for (const auto &item: rowSum) {
        feature.push_back(item);
    }
    auto colSum = finColSum();
    for (const auto &item: colSum) {
        feature.push_back(item);
    }

}

void circuit::standardize() {
    simplify();
    auto rowSum = getRowSum();
    swapRow(rowSum);
    auto colSum = getColSum();
    swapCol(colSum);

}

void circuit::swapRow(const vector<pair<int, int>> &sum) {
    /// put all the rows according to sorted sum into tempForm
    for (int i = 0; i < simplifiedRowNum; ++i) {
        int row = sum[i].first;
        auto temp = new vector<int>;
        for (const auto &item: *simplifiedTable[row]) {
            temp->push_back(item);
        }
        tempForm.push_back(temp);
    }
}

void circuit::swapCol(const vector<pair<int, int>> &sum) {
    /// put all the cols according to sorted sum into canonicalForm

    for (int i = 0; i < simplifiedRowNum; ++i) {

        auto temp = new vector<int>;
        for (int j = 0; j < inputs+outputs; ++j) {
            int col = sum[j].first;
            temp->push_back(tempForm[i]->at(col));
        }
        canonicalForm.push_back(temp);
    }

}

vector<pair<int, int>> circuit::getRowSum() const {
    vector<pair<int, int>> sum;
    int index = 0;
    for (const auto &table: simplifiedTable) {
        int temp = 0;
        for (const auto &i: *table) {
            temp += i;
        }
        sum.push_back({index, temp});
        index++;
    }
    sort(sum.begin(), sum.end(), [this](const auto &row1, const auto &row2) {
        if (row1.second <= row2.second) {
            if (row1.second < row2.second) {
                return true;
            } else {
                /// two row have the same number of 1. Special measure will be implemented.
                auto same1 = simplifiedTable[row1.first];
                auto same2 = simplifiedTable[row2.first];
                for (int i = 0; i < inputs + outputs; ++i) {
                    if (same1[i] < same2[i]) {
                        return true;
                    } else if (same1[i] > same2[i]) {
                        return false;
                    } else {
                        continue;
                    }
                }
                if (row1.first<row2.first){
                    return true;
                }else{
                    return false;
                }
            }
        } else {
            return false;
        }
    });
    return sum;
}

vector<pair<int, int>> circuit::getColSum() const {
    vector<pair<int, int>> sum;
    int index = 0;
    for (int i = 0; i < inputs + outputs; i++) {
        int temp = 0;
        for (const auto &table: tempForm) {
            temp += table->at(i);
        }
        sum.push_back({index, temp});
        index++;
    }
    vector<pair<int, int>> inputSum(sum.begin(),sum.begin()+ inputs);

    vector<pair<int, int>> outputSum(sum.begin() + inputs, sum.end());
    auto myCompare = [this](const auto &col1, const auto &col2){
        if (col1.second <= col2.second) {
            if (col1.second < col2.second) {
                return true;
            } else {
                /// two col have the same number of 1. Special measure will be implemented.
                for (int i = 0; i < simplifiedRowNum; ++i) {

                    int temp1 = tempForm[i]->at(col1.first);
                    int temp2 = tempForm[i]->at(col2.first);
                    if (temp1 < temp2) {
                        return true;
                    } else if (temp1 > temp2) {
                        return false;
                    } else {
                        continue;
                    }
                }
                if (col1.first<col2.first){
                    return true;
                }else{
                    return false;
                }
            }
        } else {

            return false;
        }
    };
    sort(inputSum.begin(), inputSum.end(), myCompare);
    sort(outputSum.begin(), outputSum.end(), myCompare);
    sum.clear();
    //sum.reserve( inputSum.size() + outputSum.size() ); // preallocate memory
    sum.insert( sum.end(), inputSum.begin(), inputSum.end() );
    sum.insert( sum.end(), outputSum.begin(), outputSum.end() );
    return sum;

}

void circuit::printCircuit() const {
    cout << "Truth Table: " << endl;
    for (const auto &i: truthTable) {
        for (const auto &j: *i) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void circuit::printSimplified() const {
    cout << "Inputs: " << inputs << " Outputs: " << outputs << endl;
    cout << "printSimplified: " << endl;
    for (const auto &i: simplifiedTable) {
        for (const auto &j: *i) {
            cout << j << " ";
        }
        cout << endl;
    }
}

void circuit::printCanonicalForm() const {
    cout << "printCanonicalForm: " << endl;
    for (const auto &i: canonicalForm) {
        for (const auto &j: *i) {
            cout << j << " ";
        }
        cout << endl;
    }
}

void circuit::printTempForm() const {
    cout << "printTempForm: " << endl;
    for (const auto &i: tempForm) {
        for (const auto &j: *i) {
            cout << j << " ";
        }
        cout << endl;
    }
}

vector<int> circuit::finRowSum() const {
    vector<int> sum;
    for (const auto &table: canonicalForm) {
        int temp = 0;
        for (const auto &i: *table) {
            temp += i;
        }
        sum.push_back(temp);
    }
    return sum;
}

vector<int> circuit::finColSum() const {
    vector<int> sum;
    for (int i = 0; i < inputs + outputs; i++) {
        int temp = 0;
        for (const auto &table: canonicalForm) {
            temp += table->at(i);
        }
        sum.push_back(temp);
    }
    return sum;
}


ostream &operator<<(ostream &str, const unordered_map<vector<int>, circuit,myHashEqualClass,myHashEqualClass>& DB) {
    str << "Print all the circuits in the database" << endl;
    for (const auto &item: DB) {
        auto circuit = item.second;
        str << "Inputs: " << circuit.inputs << " Outputs: " << circuit.outputs << endl;
        str << "Truth Table: " << endl;
        for (const auto &i: circuit.truthTable) {
            for (const auto &j: *i) {
                str << j << " ";
            }
            str << endl;
        }
    }
    str << endl;
    return str;
}

bool findCircuit(int input, int output, vector<vector<int> *>&c, unordered_map<vector<int>, circuit, myHashEqualClass, myHashEqualClass> &DB) {
    circuit circuitTobeFind(input,output,c);
    auto got = DB.find(circuitTobeFind.feature);

    if (got == DB.end())
        return false;
    else{
        return true;
    }
}


bool myHashEqualClass::operator()(const vector<int> &c1, const vector<int> &c2) const {
    if (c1==c2){
        cout << "The circuit is already in DB." << endl;
        return true;
    } else{
        return false;
    }
}

size_t myHashEqualClass::operator()(const vector<int> &feature) const {
    hash<int> h1;
    hash<size_t> h2;
    size_t sum = 0;
    for (const auto &f: feature) {
        sum+=h1(f);
    }
    return h2(sum);
}
