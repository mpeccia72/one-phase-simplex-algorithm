// CS 309 Linear Programming Project
// file description: Tableau header file

#include <iostream>
#include <iomanip>

using namespace std;

class Tableau {
    
    // 2d array that represents tableau
    double** matrix;
    
    // matrix dimensions
    int rows, cols, variables;
    
    // matrix co-ordinates 
    int currentRow, currentCol;
    
    // helper functions
    void performRowOperation(double*, double*, int bvIndex);
    void reduceRow(double*, int bvIndex);
    void determinePhase();
    
    bool isOnePhase;
    
    public:
    
        Tableau(int, int, int);
        
        void addValueToTableau(double);
        
        void findSolution();
    
        void printTableau();
};