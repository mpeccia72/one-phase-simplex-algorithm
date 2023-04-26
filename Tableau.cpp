// CS 309 Linear Programming Project
// file description: Tableau implementation file

#include "Tableau.h"

// constructor
Tableau::Tableau(int x, int y, int z) {
    
    // assignment
    rows = x;
    cols = y;
    variables = z;
    currentRow = 0;
    currentCol = 0;
    
    // allocates 2d array
    matrix = new double*[rows];
    
    for(int i = 0; i < rows; i++) {
        matrix[i] = new double[cols];
    }

}

// determines if its one phase or two phase
void Tableau::determinePhase() {
    
    isOnePhase = true;
    
    for(int i = 0; i < rows; i++) {
        
        if(matrix[i][cols - 1] < 0) {
            isOnePhase = false;
        }
      
    }
    
}

// finds solution
void Tableau::findSolution() {
    
    // will set isOnePhase variable to either true or false
    determinePhase();
    
    // counting itereations
    int iter = 0;
    
    if(isOnePhase == false) {
        cout << "Can't be solved in one phase, need Two-Phase Simplex Algorithm." << endl;
    }
    
    else {
        
        // satisfied becomes true once all values in objective function are above 0
        bool satisfied = false;
        
        while(satisfied == false) {
            
             // if infeasibleTracker is equal to rows - 1, a circuit breaker is triggered
            int infeasibleTracker = 0;
            
            // lowest real variable in objective function and its index
            double low = matrix[0][0];
            int index = 0;
        
            // finds lowest variable value in objective function
            for(int i = 1; i < cols; i++) {
        
                if(matrix[0][i] < low) {
                    low = matrix[0][i];
                    index = i;
                }
        
            } 
            
            // second part determines what row to use as pivot, currentLow is set to a high value initially for conveniency
            int choosenRowIndex = 0;
            double currentLow =  9999999;
    
            for(int i = 1; i < rows; i++) {
                
                // checks to see if the ratio between the RHS and basic variable in each row are positive and defined
                if(matrix[i][index] != 0 && matrix[i][cols - 1]/matrix[i][index] < currentLow && matrix[i][cols - 1]/matrix[i][index] > 0) {
                    currentLow = matrix[i][cols - 1]/matrix[i][index];
                    choosenRowIndex = i;
                }
                
                else {
                    infeasibleTracker++;
                }
        
            }
            
            // performs row operations from the choosen row to all the other rows
            for(int i = 0; i < rows; i++) {
        
                if(i != choosenRowIndex) {
                    performRowOperation(matrix[choosenRowIndex], matrix[i], index);
                }
            
            }
            
            // reduces choosen row
            reduceRow(matrix[choosenRowIndex], index);
            
            
            // checks if matrix is satisfied
            satisfied = true;
            
            for(int i = 0; i < cols; i++) {
                
                if(matrix[0][i] < 0) {
                    satisfied = false;
                    break;
                }
                
            }
            
            if(infeasibleTracker == rows - 1) {
                cout << "Solution is determined to be infeasible after the iteration number " << iter;
                return;
            }
            
            iter++;
            
            cout << "Iteration " << iter << endl << "-----------" << endl;
            
            printTableau();
            
        }
        
        cout << "Optimal Value: " << matrix[0][cols - 1] << endl;
        
        for(int i = 0; i < rows; i++) {
            
            for(int j = 0; j < variables; j++) {
                
                if(matrix[i][j] == 1) {
                    cout << "x^" << j + 1 << ": " << matrix[i][cols - 1] << endl;
                }
                
            }
            
        }
        
    }
    
}

// algorithm used for adding values to the tableau, fills each row before moving to next
void Tableau::addValueToTableau(double val) {
    
    // sets current element of matrix to val
    matrix[currentRow][currentCol] = val;
    
    // increments to next element 
    currentCol++;
    
    // if row is full, it increments to the next row
    if(currentCol == cols) {
        currentCol = 0;
        currentRow++;
    }
    
}

// this function performs a row operation given two rows and a basic variable index
void Tableau::performRowOperation(double* pivot, double* pivotVictim, int basicVariableIndex) {
    
    // basic variable co-efficient in pivot row
    double basicVariableCo = pivot[basicVariableIndex];
    
    // the factor that the pivot has to be multiplied by to be added to the pivotVictim row
    double factor = -(pivotVictim[basicVariableIndex]/basicVariableCo);
    
    for(int i = 0; i < cols; i++) {
        pivotVictim[i] = pivotVictim[i] + pivot[i] * factor;
    }
    
}

// reduces row so basic variable co-efficient is equal to 1 
void Tableau::reduceRow(double* pivot, int basicVariableIndex) {
    
    // basic variable co-efficient in pivot row
    double basicVariableCo = pivot[basicVariableIndex];
    
    for(int i = 0; i < cols; i++) {
        pivot[i] = pivot[i]/basicVariableCo;
    }
    
}

// prints out tableau to console
void Tableau::printTableau() {
    
    cout << " ";
    
    for(int i = 0; i < variables; i++) {
        cout << "x^" << i + 1 << "   |   ";
    }
    
    for(int i = 0; i < rows - 1; i++) {
        cout << "s^" << i + 1 << "   |   ";
    }
    
    cout << "RHS" << endl;
    
    for(int i = 0; i < rows; i++) {
        
        for(int j = 0; j < cols; j++) {
            
            if(matrix[i][j] >= 0) {
                
                if(matrix[i][j] == static_cast<int>(matrix[i][j])) {
                    cout << " " << left << fixed << setprecision(0) << setw(9) << matrix[i][j];
                }
                else {
                    cout << " " << left << fixed << setprecision(2) << setw(9) << matrix[i][j];
                }
                
            }
            
            else {
                
                if(matrix[i][j] == static_cast<int>(matrix[i][j])) {
                    cout << left << fixed << setprecision(0) << setw(10) << matrix[i][j];
                }
                else {
                    cout << left << fixed << setprecision(2) << setw(10) << matrix[i][j];
                }
                
            }
            
        }
        

        
        cout << endl;
        
    }
    
    cout << endl;
    
}
