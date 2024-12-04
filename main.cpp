#include "backendFunctionality.h"

/*
 * TODO: MOST UP-TO-DATE
 * 1. For Sam: just needs to find a way to test his functions and add some conditional statement that ensures we are only returning elements from the table that have a species name available. Don't forget to track how long each sort takes. If you finish early and have time: fix Alex's issues then Soma's
 * 2. For Alex: center userInput, have the blue outline turn black when user presses enter, and redownload csv file from zip in main and delete first row so that calculate_distance() works
 * 3. For Soma: start working on results window. Use dummy values that would mimic actual results for texting. Also display the time that each sort takes.
 */

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
    // Sets area behind prompt to grey for aesthetics
    sf::RectangleShape promptBackground;
    promptBackground.setFillColor(sf::Color(235, 235, 235));
    promptBackground.setPosition(70, 489);
    promptBackground.setSize(sf::Vector2f(359, 18));
    // Load font for text
    sf::Font arial;
    if (!arial.loadFromFile("Arial.ttf")) {
        cout << "Error loading font!!!" << endl;
        return -1;
    }
    // Prompt user
    sf::Text prompt("How many nearby unique species do you want to find?", arial, 14);
    prompt.setFillColor(sf::Color::Black);
    prompt.setPosition(70, 489);
    // Set up a text box for user to input data
    sf::RectangleShape inputBackground;
    inputBackground.setFillColor(sf::Color::White);
    inputBackground.setOutlineColor(sf::Color::Black);
    inputBackground.setOutlineThickness(3);
    inputBackground.setPosition(73, 512);
    inputBackground.setSize(sf::Vector2f(89.75, 18));
    // Used to display user's input
    sf::Text inputText("", arial, 14);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(74.5, 512);
    // Test used to display results
    sf::Text resultText("Test", arial, 14);
    resultText.setFillColor(sf::Color::Black);
    resultText.setPosition(5, 5);
    // Create an instance of backend
    Backend &backend = Backend::getInstance();
    // Dynamically allocated to avoid stack overflow error. Holds all data from the CSV.
    unordered_map<string, vector<string>> *squamates = new unordered_map<string, vector<string>>;
    // Holds latitude and longitude values of mouse
    pair<double, double> latAndLong;
    // Store data from CSV to map
    backend.createMap("squamata.csv", squamates);
    for (auto pair : *squamates) {
        if (pair.second[1] == "") {
            throw std::runtime_error("Error: DOESN'T HAVE NAME");
        }
    }
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
    // This string will hold user's input
    string input;
    // Keeps track of whether user is using the text box
    bool inputBackgroundSelected = false;
    while (window.isOpen()) {
        sf::Event evnt;
        // Handling events
        while (window.pollEvent(evnt)) {
            sf::Vector2i mouseCoords = sf::Mouse().getPosition(window);
            // Close window
            if (evnt.type == sf::Event::Closed) {
                window.close();
            }
            // User pressed on window
            if (evnt.type == sf::Event::MouseButtonPressed) {
                // User selected text box
                if (inputBackground.getGlobalBounds().contains(float(mouseCoords.x), float(mouseCoords.y))) {
                    inputBackgroundSelected = true;
                    inputBackground.setOutlineColor(sf::Color::Blue);
                }
                // User didn't select text box
                else {
                    inputBackgroundSelected = false;
                    inputBackground.setOutlineColor(sf::Color::Black);
                }
                // User selected a spot on the map
                if (!inputBackgroundSelected) {
                    cout << "Mouse Coords: " << mouseCoords.x << ", " << mouseCoords.y << endl;
                    // Convert mouse coords from xy to latitude/longitude
                    latAndLong = backend.coordConvert(mouseCoords.x, mouseCoords.y);
                    cout << "Latitude: " << latAndLong.first << " Longitude: " << latAndLong.second << endl;
                    // Calculating the distances from clicked coordinates to every reptile
                    vector<pair<string, double>> distances;
                    for (const auto &pair: *squamates) {
                        distances.push_back(make_pair(pair.first, backend.calculate_distance(latAndLong, pair.second[2], pair.second[3])));// push_back(pair.first,backend.calculate_distance(latAndLong, pair.second[2], pair.second[3]));
                    }
                    // backend.heapSort
                    // backend.quickSort
                    // Create a new window to display results
                    sf::RenderWindow resultWindow(sf::VideoMode(1224, 656), "Results of Search",sf::Style::Close | sf::Style::Titlebar);
                    sf::Event evt;
                    while (resultWindow.isOpen()) {
                        // Handling events in results window
                        while (resultWindow.pollEvent(evt)) {
                            // User closed results window
                            if (evt.type == sf::Event::Closed) {
                                resultWindow.close();
                            }
                        }
                        // Render to result window
                        resultWindow.clear(sf::Color::White);
                        resultWindow.draw(resultText);
                        resultWindow.display();
                    }
                }
            }
            // User is entering text into text box
            if (inputBackgroundSelected && evnt.type == sf::Event::TextEntered) {
                // User hit backspace
                if (evnt.text.unicode == '\b') {
                    if (!input.empty()) {
                        input.pop_back();
                    }
                }
                // User hit enter key
                else if (evnt.text.unicode == 13) {
                    inputBackgroundSelected = false;
                    inputBackground.setOutlineColor(sf::Color::Black);
                    // Sets k value used in sorting algorithms. If user enters a blank value then last saved k value is used.
                    if (!input.empty())
                        backend.setK(stoi(input));
                    cout << "K value: " << backend.getK() << endl;
                }
                // User is inputing digits. Also handles errors such as when user enters a number that is too great to be stored in an int, or when user's input exceeds bound of text box.
                else if (isdigit(char (evnt.text.unicode)) && inputText.getGlobalBounds().width < inputBackground.getSize().x - 8 && input.size() < 5) {
                    input += char (evnt.text.unicode);
                }
                // Update text object to display the user's input
                inputText.setString(input);
            }
        }
        // Render to main window
        window.clear();
        window.draw(mapSprite);
        window.draw(promptBackground);
        window.draw(prompt);
        window.draw(inputBackground);
        window.draw(inputText);
        window.display();
    }
        // Clean up heap
        delete squamates;
        return 0;
}