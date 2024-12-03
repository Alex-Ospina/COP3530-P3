#include <iostream>
#include "backendFunctionality.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <cmath>


/*
 * TODO:
 * 1. Function to read through CSV and store every value in a unsorted_map[gBifID][vector<string> which contains relevent info such as scientific name, longitude/latitude, occurenceID]
 * 2. Function to convert mouse coords to geographic coords (import libraries for greater accuracy? or just do equation for faster but less accurate results)
 *      a. Create bounds so we can throw error messages is user clicks out of bounds, optional
 * 3. Function to find kth smallest (k is user defined number of unique species) using heap. Theoretical complexity O(nlogn)
 *      a. return a vector<pairs<gBifID, distance>>
 * 4. Function to find kth smallest using shell sort. Theoretical complexity O(n^some fraction)
 *      a. first create a function that iterates through entire map and returns a vector<pair<gBifID, distance>>
 *      b. Call shell sort on this vector
 *      c. return kth smallest values in sorted vector
 */

int main() {
    unordered_map<string, vector<string>>* squamates = new unordered_map<string, vector<string>>;
    create_map("squamata-dataCondensed.csv", squamates);


    // Create a window with dimensions fitted to our chosen WGS84 map of the continental US (WGS84 map was chosen for simplicity in calculating longitude/latitude coords from mouse pos)
    sf::RenderWindow window(sf::VideoMode(1224, 656), "Reptile Finder", sf::Style::Close | sf::Style::Titlebar);
    sf::Texture mapTexture;
    // Making sure image in loaded in correctly
    if (!mapTexture.loadFromFile("WGS84_USA.png"))
        cout << "Error loading map image!!!";
    else
        mapTexture.loadFromFile("WGS84_USA.png");
    sf::Sprite mapSprite;
    mapSprite.setTexture(mapTexture);
    while (window.isOpen()) {
        sf::Event evnt;
        // Handling events
        while (window.pollEvent(evnt)) {
            if (evnt.type == sf::Event::Closed) {
                window.close();
            }
            if (evnt.type == sf::Event::MouseLeft) {
                sf::Vector2i mouseCoords = sf::Mouse().getPosition(window);
                
            }
        }
        window.clear();
        window.draw(mapSprite);
        window.display();
    }

    //clicks is returned from the image as a pair
    vector<pair<string, double>> distances;

    //calculate the distances and create
    for (const auto& pair : *squamates) {
        distances.emplace_back(pair.first, calculate_distance(clicks, pair.second[2], pair.second[3]));
    }

    delete squamates;
    return 0;
}
