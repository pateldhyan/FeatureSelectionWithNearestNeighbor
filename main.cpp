#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

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

double EuclidianDistance(vector<double> vec1, vector<double> vec2){
    double squaredSum = 0;
    for(int i = 0; i < vec1.size(); i++){
        double temp = vec2.at(i) - vec1.at(i);
        squaredSum += temp * temp; 
    }
    return sqrt(squaredSum);
}

double CrossValidation(vector<vector<double>> data, vector<int> currentFeatures, int featureToAdd){
    //Remove irrelavent features
    currentFeatures.push_back(featureToAdd);
    for (int i = 0; i < currentFeatures.size(); i++){
        int tempFeature = currentFeatures.at(i);
        for(int j = 0; j < data.size(); j++){
            data.at(j).at(tempFeature) = 0; 
        } 
    }

    int correctCount = 0;
    for(int i = 0; i < data.size(); i++){
        //Extract one object and seperate features from class 
        vector<double> objectToClassify (data.at(i).begin()+1,data.at(i).end());
        double label_objectToClassify = data.at(i).at(0);

        double nearestNeighborDistance = INT_MAX;
        int nearestNeighborLocation = INT_MAX;

        //Check euclidian distance between all other data points
        for(int j = 0; j < data.size(); j++){
            if(i != j){
                //Find euclidian distance between objects
                vector<double> testObjectFeatures (data.at(j).begin()+1,data.at(j).end());
                double distance = EuclidianDistance(objectToClassify,testObjectFeatures); 
                
                //Store new object if min euclidian distance
                if(distance < nearestNeighborDistance){
                    nearestNeighborDistance = distance;
                    nearestNeighborLocation = j;
                }
            }
        }
        //count how many labels were correctly found
        int nearestNeighborLabel = data.at(nearestNeighborLocation).at(0);
        if (label_objectToClassify == nearestNeighborLabel){
            correctCount += 1;
        }    
    }
    //Calculate accuracy
    double accuracy = static_cast<double>(correctCount) / data.size();
    return accuracy;
}

void FeatureSearch(vector<vector<double>> data){
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
    vector<int> currFeatures = {1};
    int featureToAdd = 2; 
    vector<vector<double>> data = DataImport();
    double i = CrossValidation(data, currFeatures, featureToAdd);
    cout << "Accuracy is " << i << endl;

    return 0;
}