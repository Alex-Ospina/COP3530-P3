#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

class Backend {
    Backend();
public:
    // Allows access to Backend functions
    static Backend& getInstance();
    // Iterates through and stores data from csv file into an unordered_map[gBifID][vector<scientific name, longitude, latitude, etc>]
    unordered_map<string, vector<string>> storeData(string filename);
    // Translates mouse's xy coordinates to cooresponding longitude/latitude coordinates scaled to our chosen map
    pair<float, float> coordConvert(string xPos, string yPos); // Keep in mind that return type and arguments have to work with SFML

};
