#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>

using namespace std;

class Backend {
    Backend();
    int k = 10;
public:
    // Allows access to Backend functions
    static Backend& getInstance();
    // Iterates through and stores data from csv file into an unordered_map[gBifID][vector<scientific name, longitude, latitude, etc>]
    unordered_map<string, vector<string>> storeData(string filename);
    // Translates mouse's xy coordinates to cooresponding longitude/latitude coordinates scaled to our chosen map
    pair<float, float> coordConvert(string xPos, string yPos); // Keep in mind that return type and arguments have to work with SFML

    //Likely to be the faster option as the second half terminates after k elements have been popped from the heap
    vector<string>& heapSort(int k, const vector<pair<string, float>>& distances);
    //Of the sorting algorithms that we have learned, quick sort is considered the fastest.
    vector<string>& quickSort(int k, const vector<pair<string, float>>& distances);
    int partition(vector<float> sub, int low, int high);

    void setK(int k);
};
