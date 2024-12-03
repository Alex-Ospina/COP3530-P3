#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

using namespace std;

void create_map(string filename, unordered_map<string, vector<string>>* map) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open the file.");
    }

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
        (*map)[key] = values;
    }
    file.close();
    cout << "file was processed" << endl;
}

float calculate_distance(float lat_click, float long_click, string lat_record, string long_record) {
    //dummy function
}

int main() {
    unordered_map<string, vector<string>>* squamates = new unordered_map<string, vector<string>>;
    create_map("squamata-dataCondensed.csv", squamates);
    //get the coordinates from the map image
    /*float latitude = 0.0; //dummy
    float longitude = 0.0; //dummy
    vector<pair<string, float>> distances;*/

    //calculate the distances and create
    /*for (const auto& pair : *squamates) {
        distances.emplace_back(pair.first, calculate_distance(latitude, longitude, pair.second[2], pair.second[3]));
    }*/

    delete squamates;
    return 0;
}