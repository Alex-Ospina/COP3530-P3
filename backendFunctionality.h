#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <cmath>

using namespace std;

class Backend {
    // Constructor
    Backend();
    int k = 10;
    double toRadians(double degrees); // helper for calculation
    static constexpr double EARTH_RADIUS = 6371.0;
public:
    //calculates distance from click to coordinates of records
    double calculate_distance(pair<double, double> clicks, string lat_record, string long_record);
    // Allows access to Backend functions
    static Backend& getInstance();
    // Translates mouse's xy coordinates to cooresponding longitude/latitude coordinates scaled to our chosen map
    pair<double, double> coordConvert(int xPos, int yPos); // Keep in mind that return type and arguments have to work with SFML; pair.first = latitude pair.second = longitude
    // Iterates through and stores data from CSV file into an unordered_map[gBifID][vector<scientific name, longitude, latitude, etc>]
    void createMap(string filename, unordered_map<string, vector<string>>* map);
    //Likely to be the faster option as the second half terminates after k elements have been popped from the heap
    vector<string> heapSort(const vector<pair<string, float>>& distances);
    //Of the sorting algorithms that we have learned, quick sort is considered the fastest.
    vector<string> quickSort(vector<pair<string, float>>& distances, int low, int high);
    void setK(int k);
    int getK();
};
