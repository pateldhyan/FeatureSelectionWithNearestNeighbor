#include <vector>
#include <iostream>
#include <random>

using namespace std;

// vector<int> DataImport(){
//     //Include data import code
//     //ASCII text, 8 place floating numbers
//     //First column indicates class (1 or 2)
//     //Other columns contain features
// }

int CrossValidation(vector<int> data, vector<int> currentFeatures, int featureToAdd){
    // Returns random number for testing
    // Random number generator taken from https://www.geeksforgeeks.org/how-to-generate-random-number-in-range-in-cpp/
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 100);
    int accuracy = distrib(gen);
    return accuracy;
}

void FeatureSearch(vector<int> data){
    vector<int> featuresAdded(4,0);     //0 indicates not in current set yet, 1 indicates in set

    for(int i = 1; i < data.size(); i++){
        cout << "On level " << i << " of the search tree: " << endl;
        int featureToAdd = -1;
        int maxAccuracy = 0;

        for(int j = 1; j < data.size(); j++){
            if(featuresAdded.at(j-1) == 0){
                cout << "-- Consider adding feature " << j << endl;
                int accuracy = CrossValidation(data, featuresAdded, j);
                
                if(accuracy > maxAccuracy){
                    maxAccuracy = accuracy;
                    featureToAdd = j;
                }
            }
        }
        featuresAdded.at(featureToAdd-1) = 1;
        cout << "On level " << i << ", feature " << featureToAdd << " was added to current set." << endl;
    } 
}

int main(){
    vector<int> data = {0, 0, 0, 0, 0};
    FeatureSearch(data);
    return 0;
}