#include "backendFunctionality.h"

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

    sf::RectangleShape promptBackground;
    promptBackground.setFillColor(sf::Color(235, 235, 235));
    promptBackground.setPosition(70, 489);
    promptBackground.setSize(sf::Vector2f (359, 18));

    sf::Font arial;
    if (!arial.loadFromFile("Arial.ttf")) {
        cout << "Error loading font!!!" << endl;
        return -1;
    }

    sf::Text prompt("How many nearby unique species do you want to find?", arial, 14);
    prompt.setFillColor(sf::Color::Black);
    prompt.setPosition(70, 489);

    sf::RectangleShape inputBackground;
    inputBackground.setFillColor(sf::Color::White);
    inputBackground.setOutlineColor(sf::Color::Black);
    inputBackground.setOutlineThickness(3);
    inputBackground.setPosition(73, 512);
    inputBackground.setSize(sf::Vector2f (89.75, 18));

    sf::Text inputText("", arial, 14);
    inputText.setFillColor(sf::Color::Black);






    Backend& backend = Backend::getInstance();
    // Dynamically allocated to avoid stack overflow error. Holds all data from the CSV.
    unordered_map<string, vector<string>>* squamates = new unordered_map<string, vector<string>>;
    // Holds latitude and longitude values of mouse
    pair<double, double> latAndLong;
    // Store data from CSV to map
    backend.createMap("squamata-dataCondensed.csv", squamates);
    // Create a window with dimensions fitted to our chosen WGS84 map of the continental US (WGS84 map was chosen for simplicity in calculating longitude/latitude coords from mouse position)
    sf::RenderWindow window(sf::VideoMode(1224, 656), "Reptile Finder", sf::Style::Close | sf::Style::Titlebar);
    sf::Texture mapTexture;
    // Making sure image in loaded in correctly
    if (!mapTexture.loadFromFile("WGS84_USA.png")) {
        cout << "Error loading map image!!!";
        return -1;
    }
    else
        mapTexture.loadFromFile("WGS84_USA.png");
    sf::Sprite mapSprite;
    mapSprite.setTexture(mapTexture);
    while (window.isOpen()) {
        sf::Event evnt;
        // Handling events
        while (window.pollEvent(evnt)) {
            sf::Vector2i mouseCoords = sf::Mouse().getPosition(window);
            // Close window
            if (evnt.type == sf::Event::Closed) {
                window.close();
            }
            // User selected a spot on map
            if (evnt.type == sf::Event::MouseButtonPressed) {
                cout << "Mouse Coords: " << mouseCoords.x << ", " << mouseCoords.y << endl;
                // Convert mouse coords from xy to latitude/longitude
                latAndLong = backend.coordConvert(mouseCoords.x, mouseCoords.y);
                cout << "Latitude: " << latAndLong.first << " Longitude: " << latAndLong.second << endl;
                // calculating the distances from clicked coordinates
                vector<pair<string, double>> distances;
                for (const auto& pair : *squamates) {
                    distances.emplace_back(pair.first, backend.calculate_distance(latAndLong, pair.second[2], pair.second[3]));
                }
                // backend.heapSort
                // backend.quickSort
                sf::RenderWindow resultWindow(sf::VideoMode(1224, 656), "Results of Search", sf::Style::Close | sf::Style::Titlebar);
                sf::Event evt;
                while(resultWindow.isOpen()) {
                    while (resultWindow.pollEvent(evt)) {
                        if (evt.type == sf::Event::Closed) {
                            resultWindow.close();
                        }
                    }
                }
            }
        }
        window.clear();
        window.draw(mapSprite);
        window.draw(promptBackground);
        window.draw(prompt);
        window.draw(inputBackground);
        window.display();
    }
    // Clean up heap
    delete squamates;
    return 0;
}
