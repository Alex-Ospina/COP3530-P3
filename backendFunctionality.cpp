#include "backendFunctionality.h"


//Backend &Backend::getInstance() {
//    static Backend instance;
//    return instance;
//}

//pair<float, float> Backend::coordConvert(std::string xPos, std::string yPos) {
//
//}

void Backend::create_map(string filename, unordered_map<string, vector<string>>* map) {
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

double Backend::toRadians(double degrees) {
    return degrees * M_PI / 180.0;
}
double Backend::calculate_distance(pair<double, double> clicks, string lat_record, string long_record) {
    //adapted from: https://www.geeksforgeeks.org/haversine-formula-to-find-distance-between-two-points-on-a-sphere/
    double lat1 = toRadians(clicks.first);
    double lon1 = toRadians(clicks.second);
    double lat2 = toRadians(stod(lat_record));
    double lon2 = toRadians(stod(long_record));

    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;

    double a = sin(dLat / 2.0) * sin(dLat / 2.0) +
               cos(lat1) * cos(lat2) *
               sin(dLon / 2.0) * sin(dLon / 2.0);

    double c = 2.0 * atan2(std::sqrt(a), sqrt(1.0 - a));

    return EARTH_RADIUS * c;
}