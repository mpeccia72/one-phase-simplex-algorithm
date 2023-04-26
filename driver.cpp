// CS 309 Linear Programming Project
// file description: driver file

#include <iostream>
#include <fstream>
#include <sstream>
#include "Tableau.h"

using namespace std;

// precondition: file named "input.txt" exists
// postcondition: returns the number of lines from the file "input.txt"
int countLines();

// precondition: file named "input.txt" exists and line count from that file is used as an argument
// postcondition: returns Tableau object that read data from "input.txt"
Tableau readData(int);

// precondition: a string is inputted
// postcondition: the string is returned without "<=" or ">="
string delimit(string);

int main()
{
    
    cout << "One Phase Simplex Calculator" << endl << "----------------------------" << endl;
    
    int lines = countLines();
    
    Tableau myTab = readData(lines);
    
    myTab.printTableau();
    
    char ans;
    
    cout << "Would you like to find optimal solution (Y for yes)? ";
    cin >> ans;
    
    if(ans != 'y' && ans != 'Y') {
        cout << endl << "Bye!";
        exit(0);
    }
    
    cout << endl;
    
    myTab.findSolution();

    cout << endl << endl << "Enter any key to exit.";
    cin >> ans;
    
    
}

int countLines() {
    
    // creates input file object and opens a file called "input.txt"
    ifstream input;
    input.open("input.txt");
    
    // if file fails to open, error is caught
    if(input.fail()) {
        cout << "Failed to open 'input.txt'";
        exit(0);
    }
    
    int count = 0;
    string line;
    
    // loop that goes through each line 
    while(getline(input, line)) {
        if(line.empty()) break;
        count++;
    }
    
    // closes file
    input.close();
    
    return count;
    
}

string delimit(string text) {
    
    string result = text.substr(0, text.find("=") - 1) + text.substr(text.find("=") + 1);
    
    return result;

}


Tableau readData(int lines) {
    
    // creates input file object and opens a file called "input.txt"
    ifstream input;
    input.open("input.txt");
    
    // if file fails to open, error is caught
    if(input.fail()) {
        cout << "Failed to open 'input.txt'";
        exit(0);
    }
    
    // used to hold strings from file
    string line;
    
    // grabbing sample line to determine number of variables
    getline(input, line);
    istringstream sampleRowStream(line);
    
    // variables will represent the number of non-slack variables from file
    int variables = 0;
	int temp;
    while(sampleRowStream >> temp) variables++;
    
    // resets file read pointer to beginning
    input.seekg(0, ios::beg);
    
    // calculates other important variables
    int slackVariables = lines - 1;
    int cols = variables + slackVariables + 1;
    int rows = lines;
    
    // an array to keep track of slack variable signs going down the file
    int* slackVarSign = new int[rows - 1];
    
    // skips first line
    getline(input, line);
    
    // algorithm to fill array
    for(int i = 0; i < rows - 1; i++) {
        
        getline(input, line);
        
        // greater than is negative
        if(line.find(">=") != -1) {
            slackVarSign[i] = -1;
        }
        
        // less than is positive
        else {
            slackVarSign[i] = 1;
        }
        
    }
    
    // resets file read pointer to beginning
    input.seekg(0, ios::beg);

    // Tableau instantiation
    Tableau tableauObject(rows, cols, variables);
    
    // used to hold integers from file
    int num;

    // algorithm to capture objective function to matrix (first line in file)
    for(int i = 0; i < cols - slackVariables - 1; i++) {
        input >> num;
        tableauObject.addValueToTableau(-num);
    }
    
    for(int i = 0; i < slackVariables + 1; i++) {
        tableauObject.addValueToTableau(0);
    }
    
    // skips first line
    getline(input, line);
    
    // algorithm for filling the rest of the matrix
    for(int i = 0; i < rows - 1; i++) {
        
        // pulls line, delimits characters, then inserts it back into new stream
        getline(input, line);
        string numbers = delimit(line);
        istringstream iss(numbers);
        
        // fills the non-slack variables to matrix row
        for(int j = 0; j < cols - slackVariables - 1; j++) {
            
            iss >> num;
            tableauObject.addValueToTableau(num);
            
        }
        
        // fills the slack variables to matrix row
        for(int j = 0; j < slackVariables; j++) {
            
            if(i == j) {
                tableauObject.addValueToTableau(slackVarSign[i]);
            }
            
            else {
                tableauObject.addValueToTableau(0);
            }
            
        }
        
        iss >> num;
        tableauObject.addValueToTableau(num);
        
    }
    
    // closes file
    input.close();
    
    return tableauObject;
    
} 
