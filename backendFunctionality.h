#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

using namespace std;

class Backend {
    // Constructor
    Backend();
public:
    static constexpr double EARTH_RADIUS = 6371.0;
    // Allows access to Backend functions
    static Backend& getInstance();
    // Translates mouse's xy coordinates to cooresponding longitude/latitude coordinates scaled to our chosen map
    pair<double, double> coordConvert(int xPos, int yPos); // Keep in mind that return type and arguments have to work with SFML; pair.first = latitude pair.second = longitude
    // Iterates through and stores data from CSV file into an unordered_map[gBifID][vector<scientific name, longitude, latitude, etc>]
    void createMap(string filename, unordered_map<string, vector<string>>* map);
    //calculates distance from click to coordinates of records
    double toRadians(double degrees);
    double calculate_distance(pair<double, double> clicks, string lat_record, string long_record);
};
