//
//  CSP.h
//  Sudoku
//
//  Created by cpsfaculty on 10/10/14.
//  Copyright (c) 2014 ___Ju Shen___. All rights reserved.
//

#ifndef Sudoku_CSP_h
#define Sudoku_CSP_h

#include <stack>
#include <vector>
#include <queue>
#include <iostream>
#include <ctime>        // std::time
#include <cstdlib>   
#include <algorithm>
#endif

using namespace std;


/*This structure defines the variable format for the Sudoku problem*/
struct Variable{
    vector<int> domain; //the possible values of this variable, only when the varible has not been assigned yet
    int assignment; //0 means this variable has not been assigned yet
};


/*State structure: one state of assignment for the 9 by 9 variables*/
struct State{
    int values[9][9];
    
    //reload the equal check operator
    bool operator==(const State& s) const
    {   
        for(int i = 0; i < 81; i++)
        {
            int y = i / 9;
            int x = i % 9;
            if(values[y][x] != s.values[y][x])
            {
                return false;
            }
        }
        
        return true;
    }
    
};

/*Struct of arrow, which is a pair of node with directions*/
struct Arrow{
    int headrow;
	int headcol;
    int tailrow;
	int tailcol;
    
    Arrow();
    Arrow(int hr, int hc, int tr, int tc)
    {
        headrow = hr;
		headcol = hc;
        tailrow = tr;
		tailcol = tc;
    }
    
    bool operator==(const Arrow& a) const
    {
        if(headrow == a.headrow && headcol == a.headcol && tailrow == a.tailrow && tailcol == a.tailcol)
            return true;
        else
            return false;
    }
};



/*This is the class that solve the constraint problem by using the algroithms learned in the class, such as backtrack, forward checking,...*/
class CSP{
public:
    State cur_state; //it has 9*9 values inside the variable. Each value represents the assignment for the corresponding cell. If it is 0, it means no assignment for the cell yet.
    Variable variables[9][9]; //Each varible represents one cell of the sudocu table. Each variables[i][j] 
    stack<int> assigned_variables; //store the order of varaibles that have been assigned, so every time when back track happens, just need to
                                    //update the lastest variable value and update the corresponding domain sets.
    vector<State> repeating_list; //store the repeated pattern
    int alg_opt; //algorithm_option in VisualDisplay: 1 - backtrack, 2-forward checking, 3-arc consistency
    //int chosen_cell;
    int random; //decide whether to use random value from the domain
    int backtrackNum;
    

    
public:
    CSP();
    ~CSP();
	void updateDomain(State state); //for forward checking used. Based on current setting, update the domain.
    void arcConsistency(const State state); //similar to the updateDomain() in the forward checking, this unction is stronger for arcChecking use
    bool goalCheck(const State state);
    void setData(int *data);
    int goBack();
    bool backTrack();
    bool forwardChecking();
    bool forwardCheckingOrder();
    bool arcChecking();
    bool arcCheckingOrder();
    void clearData(); // this is called everytime when the search algorithm (radio button) is switched to another one.

	bool InRow(State state, int row,int col, int num);
	bool InCol(State state,int row, int col, int num);
	bool InBox(State state, int row, int col, int num);
	void addArc(int n);
	void printState();
	void nakedRules(int k);
	
  
    void reshuffleDomain(); //make the domain values in random order
    void sortDomain(); //make the domain in increasing order
};
