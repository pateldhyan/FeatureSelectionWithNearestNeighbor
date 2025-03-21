#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

//Function to handle data set import
//File I/O referenced from https://cplusplus.com/doc/tutorial/files/
// and https://cplusplus.com/reference/sstream/
vector<vector<double>> DataImport(){
    string fileName;
    cout << "Welcome to Dhyan Patel's Feature Selection Algorithm." << endl;
    cout <<  "Type in the name of the file to test:  ";
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
bool isInVec(vector<int>& v, int testInt){
    for (int i = 0; i < v.size(); i++) {
        if (v.at(i) == testInt) {
            return true; 
        }
    }
    return false;
}

//Leave-one-out cross validation
double CrossValidation(vector<vector<double>> data, vector<int> currentFeatures, int featureToAdd){

    //Remove irrelavent features
    if(featureToAdd != -1){
        currentFeatures.push_back(featureToAdd);
    }
    for (int i = 1; i < data.at(0).size(); i++){
        if(!isInVec(currentFeatures, i)){
            for(int j = 0; j < data.size(); j++){
                data.at(j).at(i) = 0; 
            }
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

void ForwardFeatureSearch(vector<vector<double>> data){
    vector<int> featuresAdded;
    double maxAccuracyOverall = -1;
    double tempAccuracy = -1;
    vector<int> bestFeatures;
    bool accuracyIncreasing = 1;

    //Iterate through each level
    for(int i = 1; i < data.at(0).size(); i++){
        int featureToAdd = -1;
        double maxAccuracy = -1;
        //Test accuracy with every unadded feature, determine which is best
        for(int j = 1; j < data.at(0).size(); j++){
            if(!isInVec(featuresAdded,j)){
                double accuracy = CrossValidation(data, featuresAdded, j);
                vector<int> temp = featuresAdded;
                temp.push_back(j); 
                cout << "   Using feature(s) { "; printVec(temp); cout <<"} accuracy is "<< accuracy * 100 << "%"<< endl;
                if(accuracy > maxAccuracy){
                    maxAccuracy = accuracy;
                    tempAccuracy = accuracy;
                    featureToAdd = j;
                }
            }
        }
        //Add best feature
        featuresAdded.push_back(featureToAdd);
        cout << endl << "Feature set { "; printVec(featuresAdded); cout << "} was best, accuracy is " << maxAccuracy * 100 << "%" << endl << endl;

        if(accuracyIncreasing == 1 && tempAccuracy < maxAccuracyOverall){
            cout << "(Warning, Accuracy has decreased. Continuing search in case of local maxima)" << endl << endl;
            accuracyIncreasing = 0;
        }
        //Keep track of best feature set so far
        if (tempAccuracy > maxAccuracyOverall){
            maxAccuracyOverall = tempAccuracy;
            bestFeatures = featuresAdded; 
        }   
    }
    
    cout << "Finished search! The best feature subset is { "; printVec(bestFeatures); cout << "}, which has an accuracy of " << maxAccuracyOverall * 100 << "%" << endl;
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
        int featureToRemove = -1;
        double maxAccuracy = -1;
    
        for(int j = 0; j < featuresAdded.size(); j++){
            int currFeature = featuresAdded.at(j);
            vector<int> tempFeatures = featuresAdded;
            tempFeatures.erase(tempFeatures.begin()+j);
            double accuracy = CrossValidation(data, tempFeatures, -1);
            cout << "   Removing feature {" << currFeature <<"} accuracy is "<< accuracy * 100 << "%" << endl;
            if(accuracy > maxAccuracy){
                maxAccuracy = accuracy;
                tempAccuracy = accuracy;
                featureToRemove = j;
            }
        }
        cout << endl <<"Feature {" << featuresAdded.at(featureToRemove) << "} was best to remove. " << endl; 
        featuresAdded.erase(featuresAdded.begin() + featureToRemove);
        cout << "The current data set is { "; printVec(featuresAdded); cout << "} "<< endl << endl;
        if(accuracyIncreasing == 1 && tempAccuracy < maxAccuracyOverall){
            cout << "(Warning, accuracy has decreased. Continuing search in case of local maxima)" << endl << endl;
            accuracyIncreasing = 0;
        }
        if (tempAccuracy > maxAccuracyOverall){
            maxAccuracyOverall = tempAccuracy;
            bestFeatures = featuresAdded; 
        }   
    }
    
    cout << "Finished search! The best feature subset was { "; printVec(bestFeatures); cout << "}, which has an accuracy of " << maxAccuracyOverall * 100 << "%." << endl;
}

int main(){
    vector<vector<double>> data = DataImport();
    cout << "Type the number of the algorithm you want to run. " << endl << endl;
    cout << "   1) Forward Selection" << endl;
    cout << "   2) Backwards Elimination" << endl;
    int userInput;
    cin >> userInput;
    int numFeatures  = data.at(0).size() -1;
    vector<int> allFeatures;
    for(int i = 1; i < data.at(0).size(); i++){
        allFeatures.push_back(i);
    }

    cout << endl << "This dataset has " << numFeatures << " features (not including the class attribute), with " << data.size() << " instances." << endl; 
    cout << endl << endl;
    cout << "Running nearest neighbor with all " << numFeatures << " features, using 'leaving-one-out' evaluation, I get an accuracy of "<<  CrossValidation(data, allFeatures, -1)*100 << "%" <<endl;
    cout << endl << "Beginning search. " << endl << endl;  

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