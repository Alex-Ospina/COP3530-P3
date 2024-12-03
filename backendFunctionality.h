#pragma once

//#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <cmath>

using namespace std;

class Backend {
    Backend();
public:
    static constexpr double EARTH_RADIUS = 6371.0;
    // Allows access to Backend functions
    static Backend& getInstance();
    // Iterates through and stores data from csv file into an unordered_map[gBifID][vector<scientific name, longitude, latitude, etc>]
    void create_map(string filename, unordered_map<string, vector<string>>* map);
    // Translates mouse's xy coordinates to cooresponding longitude/latitude coordinates scaled to our chosen map
    pair<float, float> coordConvert(string xPos, string yPos); // Keep in mind that return type and arguments have to work with SFML
    //calculates distance from click to coordinates of records
    double toRadians(double degrees);
    double calculate_distance(pair<double, double> clicks, string lat_record, string long_record);
};