#include "backendFunctionality.h"

//Backend &Backend::getInstance() {
//    static Backend instance;
//    return instance;
//}

//pair<float, float> Backend::coordConvert(std::string xPos, std::string yPos) {
//
//}

class CompareDistances{
public:
    bool operator()(const pair<string, float>& specie1, const pair<string, float>& specie2){
        return specie1.second > specie2.second;
    }
};

vector<string>& Backend::heapSort(int k, const vector<pair<string, float>> &distances) {
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

vector<string> &Backend::quickSort(const vector<pair<std::string, float>> &distances) {
    vector<string> results(k);

    return results;
}



int Backend::partition(vector<float> sub, int low, int high) {
    int pivot = sub[low];
    int up = low;
    int down = high;

}


void Backend::setK(int k) {
    this->k = k;
}
