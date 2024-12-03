#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

unordered_map<string, vector<string>> create_map(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open the file.");
    }

    unordered_map<string, vector<string>> map;
    string line;

    while (getline(file, line)) {
        istringstream lineStream(line);
        string key;
        string rest;
        vector<string> values;

        getline(lineStream, key, ';');
        while (getline(lineStream, rest, ';')) {
            values.push_back(rest);
        }
        map[key] = values;
    }
    file.close();
    return map;
}

int main() {
    unordered_map<string, vector<string>> squamates = create_map("squamata_test.csv");


    return 0;
}