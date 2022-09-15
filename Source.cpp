#include "myFunc.h"





int main(){
    // the first part of the unordered_map is the feature of the circuit, the second part is the circuit

    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    /* Create a connection */
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "********");
    con->setSchema("circuitStorage");

    int input1 = 3;
    int output1= 2;
    vector<vector<int> *> exactCircuit = {
            new vector<int>{0,0,0,1,0},
            new vector<int>{0,0,1,0,0},
            new vector<int>{0,1,0,0,0},
            new vector<int>{0,1,1,1,0},
            new vector<int>{1,0,0,0,0},
            new vector<int>{1,0,1,1,1},
            new vector<int>{1,1,0,0,1},
            new vector<int>{1,1,1,1,1}
    };

    int input2 = 2;
    int output2 = 1;
    vector<vector<int> *> differentCircuit = {
            new vector<int>{0,0,1},
            new vector<int>{0,1,0},
            new vector<int>{1,0,0},
            new vector<int>{1,1,1}
    };
    int input3 = 3;
    int output3= 2;
    vector<vector<int> *> sameCircuitNotExact = {
            new vector<int>{0,0, 0,0,1},
            new vector<int>{0,0, 1,0,0},
            new vector<int>{1,0, 0,0,0},
            new vector<int>{1,0, 1,0,1},
            new vector<int>{0,1, 0,0,0},
            new vector<int>{0,1, 1,1,1},
            new vector<int>{1,1, 0,1,0},
            new vector<int>{1,1, 1,1,1}
    };

    unordered_map<vector<int>, circuit,myHashEqualClass,myHashEqualClass> DB;

    string fileLocation = R"(C:\Users\xkzmx\Documents\Odyssey\School\CIS554\HW6\Circuits\circuits.txt)";

    int totalNumber = 0;
    int inputs = 0;
    int outputs = 0;
    string line;
    ifstream inFile;
    inFile.open(fileLocation);

    if (inFile.is_open()) {
        try {
            // use try catch to prevent any read in error.
            getline(inFile, line);
            totalNumber = stoi(line);
            for (int i = 0; i < totalNumber; i++) {
                getline(inFile, line);
                inputs = stoi(line);
                getline(inFile, line);
                outputs = stoi(line);
                circuit tempCircuit(inputs, outputs);
                for (int j = 0; j < tempCircuit.rowNum; j++) {
                    getline(inFile, line);
                    auto temp = new vector<int>;
                    for (int k = 0; k < inputs; k++) {
                        temp->push_back(line[k] - '0');
                    }
                    for (int m = 0; m < outputs; m++) {
                        temp->push_back(line[inputs + m + 1] - '0');
                    }
                    tempCircuit.truthTable.push_back(temp);
                }
                tempCircuit.setFeatureOfCircuit();
                DB[tempCircuit.feature] = tempCircuit;

            }
        }
        catch (...) {
            std::cout << "Read data error" << endl;
            return -1;
        }
    } else {
        cout << "Unable to open file" << endl;
        return -1;
    }
    inFile.close();

cout << DB << endl;

    //circuit 1 is the same circuit saved in the DB, will find
    if (findCircuit(input1,output1,exactCircuit,DB)){
        cout << "Can find circuit1" << endl;
    }
    //circuit 2 is a new circuit, won't find
    if (findCircuit(input2,output2,differentCircuit,DB)){
        cout << "Can find circuit2" << endl;
    }else{
        cout << "Can't find circuit2" << endl;
    }
    //circuit 3 is a new form of a circuit saved in the DB, will find
    if (findCircuit(input3,output3,sameCircuitNotExact,DB)){
        cout << "Can find circuit3" << endl;
    }else{
        cout << "Can't find circuit3" << endl;
    }

      delete con;

    return 0;
}

