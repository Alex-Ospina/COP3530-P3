#include "backendFunctionality.h"

Backend::Backend() {

}

Backend &Backend::getInstance() {
    static Backend instance;
    return instance;
}

/* map width = 1048, map height = 566
 full image width = 1224, full image height = 656
 Longitude min = -124.848974, Longitude max = -66.885444
 Latitude min = 24.396308, Latitude max = 49.384358
 Maximum and minimum latitude/longitude values for a bounding box of the continental US taken from Martijin van Exel's response in https://www.quora.com/What-is-the-longitude-and-latitude-of-a-bounding-box-around-the-continental-United-States
 */
pair<double, double> Backend::coordConvert(int xPos, int yPos) {
    // Calculate margins to compensate for difference in dimensions of the full image and the map itself
    double marginX = (1224.0 - 1048.0)/2;
    double marginY = (656.0 - 566)/2;
    // Adjust coords
    double adjX = xPos - marginX;
    double adjY = yPos - marginY;
    // Handles errors where user clicks out of bounds. Pulls user's mouse click coordinates to the closest spot in the viable area
    adjX = max(0.0, min(1048.0, adjX));
    adjY = max(0.0, min(566.0, adjY));
    // Longitude value found by using linear interpolation. Subtracted an additional 1.5 to compensate for image distortion.
    double longitude = (-124.848974 + (double(adjX)/1048.0) * (-66.885444 - (-124.848974))) - 1.5;
    // Latitude value found by using inverse linear interpolation accounting for how the y-coordinate grows as you move down the image
    double latitude = 49.384358 - (double(adjY)/566.0) * (49.384358 - 24.396308);
    // Longitude and latitude values are within a degree of their actual positions (within ~50 miles)
    return make_pair(latitude, longitude);
}

void Backend::createMap(std::string filename, unordered_map<std::string, vector<std::string>> *map) {
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
        getline(lineStream, key, ',');
        while (getline(lineStream, rest, ',')) {
            values.push_back(rest);
        }
        (*map)[key] = values;
    }
    file.close();
    std::cout << "File was processed" << endl;
}

class CompareDistances{
public:
    bool operator()(const pair<string, float>& specie1, const pair<string, float>& specie2){
        return specie1.second > specie2.second;
    }
};

vector<string> Backend::heapSort(const vector<pair<string, float>> &distances) {
    vector<string> results(k);
    auto minHeap = new priority_queue<pair<string, float>, vector<pair<string, float>>, CompareDistances>;
    for(auto specie : distances){
        minHeap->push(specie);
    }
    for(int i = 0; i < k; i++){
        auto nextClosest = minHeap->top();
        minHeap->pop();
        results[i] = nextClosest.first;
    }
    delete minHeap;
    return results;
}

int partition(vector<pair<string, float>>& sub, int low, int high) {
    float pivot = sub[low].second;
    int i = low + 1;
    int j = high;
    while(true){
        while(i <= j && sub[i].second < pivot){
            i++;
        }
        while(i <= j && sub[j].second > pivot){
            j--;
        }

        if(i >= j)
            break;

        swap(sub[i], sub[j]);
    }
    swap(sub[low], sub[j]);
    return j;

}

vector<string> Backend::quickSort(vector<pair<string, float>>& distances, int low, int high){
    vector<string> results(k);

    if(low < high){
        int pivot = partition(distances, low, high);
        quickSort(distances, low, pivot -1);
        quickSort(distances, pivot + 1, high);
    }

    for(int i = 0; i < k; i++){
        results[i] = distances[i].first;
    }
    return results;
}

void Backend::setK(int k) {
    this->k = k;
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

int Backend::getK() {
    return k;
}