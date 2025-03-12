#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

using namespace std;

//Function to handle data set import
//File I/O referenced from https://cplusplus.com/doc/tutorial/files/\
// and https://cplusplus.com/reference/sstream/
vector<vector<double>> DataImport(){
    string fileName;
    cout <<  "Enter the name of the file you would like to import" << endl;
    cin >> fileName;
    
    ifstream file(fileName);            //Open file
    vector<vector<double>> dataSet;     //Initialize 2D Vector to store data
    
    string line;                        //String to hold each row
    while(getline(file, line)){         //Parsing each string line into a double vector
        vector<double> row; 
        double value;
        istringstream strStrLine(line); 
        while(strStrLine >> value){     //Add each number to row vector
            row.push_back(value);
        }
        dataSet.push_back(row);         //Add row vector to 2D data set vector
    }
    file.close();

    return dataSet;
}

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
    vector<int> featuresAdded(data.size()-1, 0);     //0 indicates not in current set yet, 1 indicates in set

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
    vector<int> data = {0, 0, 0, 0, 0, 0, 0, 0};
    FeatureSearch(data);

    return 0;
}