#include "backendFunctionality.h"
#include <chrono>


int main() {
    long heapSortTime;
    long quickSortTime;
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
    sf::Text prompt("How many nearby species do you want to find?", arial, 14);
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

    // Text used to display results
    sf::Text heapResult("HeapSort Time Elapsed(ms)", arial, 18);
    sf::Text quickResult("QuickSort Time Elapsed(ms)", arial, 18);
    int page = 0;
    sf::Text resultText1("Test1", arial, 20);
    sf::Text resultText2("Test2", arial, 20);
    sf::Text resultText3("Test3", arial, 20);
    sf::Text resultText4("Test4", arial, 20);
    sf::Text resultText5("Test5", arial, 20);
    sf::Text resultText6("Test6", arial, 20);
    sf::Text resultText7("Test7", arial, 20);
    sf::Text resultText8("Test8", arial, 20);
    sf::Text resultText9("Test9", arial, 20);
    sf::Text label("Species", arial, 20);
    sf::Text link("Link", arial, 20);
    sf::Text coordinates("Coordinates", arial, 20);
    label.setFillColor(sf::Color::Black);
    link.setFillColor(sf::Color::Black);
    label.setPosition(25, 10);
    link.setPosition(500, 10);
    vector<sf::Text> resultTexts = {resultText1, resultText2, resultText3, resultText4, resultText5, resultText6,
                                resultText7, resultText8, resultText9};
    quickResult.setFillColor(sf::Color::Blue);
    heapResult.setFillColor(sf::Color::Red);
    coordinates.setFillColor(sf::Color::Black);
    for(int i = 0; i < resultTexts.size(); i++){
        resultTexts[i].setFillColor(sf::Color::Black);
        resultTexts[i].setPosition(25, 40 + i * 55);
    }
    sf::Texture backT;
    if(!backT.loadFromFile("back.png")){
        throw runtime_error("Cannot load back image");
    }
    sf::Texture forwardT;
    if(!forwardT.loadFromFile("forward.png")){
        throw runtime_error("Cannot load forward image");
    }
    sf::Sprite back;
    sf::Sprite forward;
    back.setTexture(backT);
    back.setScale(0.3, 0.3);
    forward.setScale(0.3, 0.3);
    forward.setPosition(1150, 580);
    forward.setTexture(forwardT);
    back.setPosition(20, 580);
    heapResult.setPosition(450, 560);
    quickResult.setPosition(450, 590);
    coordinates.setPosition(800, 10);

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

                    vector<string> results(backend.getK());

                    // Used to ensure quickSort returns an identitcal results, left here for documentation purposes
                    //vector<string> results2(backend.getK());

                    //Record amount of time elapsed in each sorting algorithm as it runs
                    chrono::time_point<std::chrono::high_resolution_clock> timerStart = std::chrono::high_resolution_clock::now();
                    results = backend.heapSort(distances);
                    heapSortTime = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock ::now() - timerStart).count();
                    timerStart = std::chrono::high_resolution_clock::now();
                    backend.quickSort(distances);
                    //We do not catch the results of quickSort since we have tested that heapSort and quickSort return identical vectors
                    //We used the following code to do so
                    /*
                     *  for(int i = 0; i < results.size(); i++){
                     *      if(results[i] != results2[i])
                     *          throw runtime_error("Different results");
                     *  }
                     */

                    quickSortTime = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock ::now() - timerStart).count();
                    heapResult.setString("Heap Sort Elapsed Time(ms) " + to_string(heapSortTime));
                    quickResult.setString("Quick Sort Elapsed Time(ms) " + to_string(quickSortTime));

                    //Assign initial result texts
                    for(int i = 0; i < resultTexts.size(); i++){
                        resultTexts[i].setString((*squamates)[results[i]][1] + "       " + (*squamates)[results[i]][0] + "     " + (*squamates)[results[i]][2] + "     " + (*squamates)[results[i]][3]);
                    }

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
                            if(evt.type == sf::Event::MouseButtonPressed){
                                sf::Vector2i mouse = sf::Mouse().getPosition(resultWindow);
                                //Handle pagination logic
                                if(back.getGlobalBounds().contains(mouse.x, mouse.y)&& page != 0) {
                                    page--;
                                    for(int i = 0; i < 9; i++) {
                                        if (page * 9 + i < results.size())
                                            resultTexts[i].setString((*squamates)[results[i + 9 * page]][1] + "     " +
                                                                 (*squamates)[results[i + 9 * page]][0] + "     " +
                                                                 (*squamates)[results[i + 9 * page]][2] + "     " +
                                                                 (*squamates)[results[i + 9 * page]][3]);
                                        else
                                            resultTexts[i].setString("");
                                    }
                                }
                                else if(forward.getGlobalBounds().contains(mouse.x, mouse.y) && page != ceil(float(backend.getK()/9.0f)) - 1) {
                                    page++;
                                    for(int i = 0; i < 9; i++){
                                        if(page * 9 + i < results.size())
                                            resultTexts[i].setString((*squamates)[results[i + 9 * page]][1] + "       " +
                                                                    (*squamates)[results[i + 9 * page]][0] + "     " +
                                                                    (*squamates)[results[i + 9 * page]][2] + "     " +
                                                                    (*squamates)[results[i + 9 * page]][3]);
                                        else
                                            resultTexts[i].setString("");
                                    }
                                }
                            }
                        }
                        // Render to result window
                        resultWindow.clear(sf::Color::White);
                        for(auto text : resultTexts){
                            resultWindow.draw(text);
                        }
                        resultWindow.draw(label);
                        resultWindow.draw(link);
                        resultWindow.draw(back);
                        resultWindow.draw(forward);
                        resultWindow.draw(heapResult);
                        resultWindow.draw(quickResult);
                        resultWindow.draw(coordinates);
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