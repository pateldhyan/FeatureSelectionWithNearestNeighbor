#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

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

void FeatureSearch(vector<int> data){
    //Insert feature search alg. Data import need to be done first
}

int main(){

    return 0;
}