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
    cout <<  "Enter the name of the file you would like to import: ";
    cin >> fileName;
    cout << endl;

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

double EuclidianDistance(vector<double>& vec1, vector<double>& vec2){
    double squaredSum = 0;
    for(int i = 0; i < vec1.size(); i++){
        double temp = vec2.at(i) - vec1.at(i);
        squaredSum += temp * temp; 
    }
    return sqrt(squaredSum);
}

//Leave-one-out cross validation
double CrossValidation(vector<vector<double>> data, vector<int> currentFeatures, int featureToAdd){
    //Remove irrelavent features
    if(featureToAdd != -1){
        currentFeatures.push_back(featureToAdd);
    }
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

void printVec(vector<int>& v){
    for (auto i : v)
        cout << i << " ";
}
bool isInVec(vector<int>& v, int testInt){
    for (int i = 0; i < v.size(); i++) {
        if (v.at(i) == testInt) {
            return true; 
        }
    }
    return false;
}
void ForwardFeatureSearch(vector<vector<double>> data){
    vector<int> featuresAdded;
    double maxAccuracyOverall = -1;
    double tempAccuracy = -1;
    vector<int> bestFeatures;
    bool accuracyIncreasing = 1;

    //Iterate through each level
    for(int i = 1; i < data.at(0).size(); i++){
        cout << "On level " << i << " of the search tree with feature set { "; printVec(featuresAdded); cout << "}" << endl;
        int featureToAdd = -1;
        double maxAccuracy = -1;
        //Test accuracy with every unadded feature, determine which is best
        for(int j = 1; j < data.at(0).size(); j++){
            if(!isInVec(featuresAdded,j)){
                double accuracy = CrossValidation(data, featuresAdded, j);
                cout << "-- Considering adding feature " << j <<", which gives an accuracy of "<< accuracy * 100 << "%."<< endl;
                if(accuracy > maxAccuracy){
                    maxAccuracy = accuracy;
                    tempAccuracy = accuracy;
                    featureToAdd = j;
                }
            }
        }
        //Add best feature
        featuresAdded.push_back(featureToAdd);
        cout << "On level " << i << ", feature " << featureToAdd << " was added to current set." << endl << endl;

        if(accuracyIncreasing == 1 && tempAccuracy < maxAccuracyOverall){
            cout << "WARNING: Accuracy is decreasing. Will complete search in case of local maxima" << endl << endl;
            accuracyIncreasing = 0;
        }
        //Keep track of best feature set so far
        if (tempAccuracy > maxAccuracyOverall){
            maxAccuracyOverall = tempAccuracy;
            bestFeatures = featuresAdded; 
        }   
    }
    
    cout << "The best feature set was { "; printVec(bestFeatures); cout << "} with an accuracy of " << maxAccuracyOverall * 100 << "%." << endl;
}
//Same as forward search, but starts with all features and removes one at a time
void ReverseFeatureSearch(vector<vector<double>> data){
    //Start with all features
    vector<int> featuresAdded;
    for(int i = 1; i < data.at(0).size(); i++){
        featuresAdded.push_back(i);
    }
    double maxAccuracyOverall = -1;
    double tempAccuracy = -1;
    vector<int> bestFeatures;
    bool accuracyIncreasing = 1;

    for(int i = 1; i < data.at(0).size(); i++){
        cout << "On level " << i << " of the search tree with feature set { "; printVec(featuresAdded); cout << "}" << endl;
        int featureToRemove = -1;
        double maxAccuracy = -1;
    
        for(int j = 0; j < featuresAdded.size(); j++){
            int currFeature = featuresAdded.at(j);
            vector<int> tempFeatures = featuresAdded;
            tempFeatures.erase(tempFeatures.begin()+j);
            double accuracy = CrossValidation(data, tempFeatures, -1);
            cout << "-- Considering removing feature " << currFeature <<", which gives an accuracy of "<< accuracy * 100 << "%." << endl;
            if(accuracy > maxAccuracy){
                maxAccuracy = accuracy;
                tempAccuracy = accuracy;
                featureToRemove = j;
            }
        }
        cout << "On level " << i << ", feature " << featuresAdded.at(featureToRemove) << " was removed from current set." << endl << endl;
        featuresAdded.erase(featuresAdded.begin() + featureToRemove);

        if(accuracyIncreasing == 1 && tempAccuracy < maxAccuracyOverall){
            cout << "WARNING: Accuracy is decreasing. Will complete search in case of local maxima" << endl << endl;
            accuracyIncreasing = 0;
        }
        if (tempAccuracy > maxAccuracyOverall){
            maxAccuracyOverall = tempAccuracy;
            bestFeatures = featuresAdded; 
        }   
    }
    
    cout << "The best feature set was { "; printVec(bestFeatures); cout << "} with an accuracy of " << maxAccuracyOverall * 100 << "%." << endl;
}

int main(){
    vector<vector<double>> data = DataImport();
    cout << "We will run 'leaving-one-out' evaluation. Would you like to use: " << endl;
    cout << "[1] Forward Selection" << endl;
    cout << "[2] Backwards Elimination" << endl;
    int userInput;
    cin >> userInput;
    int numFeatures  = data.at(0).size() -1;
    vector<int> allFeatures;
    for(int i = 1; i < data.at(0).size(); i++){
        allFeatures.push_back(i);
    }

    cout << "This data set has " << numFeatures << " features, with " << data.size() << " instances." << endl; 
    cout << "The accuracy of the nearest neighbor algorithm using all " << numFeatures << " features is " << CrossValidation(data, allFeatures, -1) << endl;
    cout << "Beginning search: " << endl;  

    if(userInput == 1){
        ForwardFeatureSearch(data);
    }
    else if(userInput == 2){
        ReverseFeatureSearch(data);
    }
    else{
        cout << "Please enter 1 or 2" << endl;
    }
    
    return 0;
}