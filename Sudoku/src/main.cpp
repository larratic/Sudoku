//
//  main.cpp
//  Sudoku
//
//  Created by cpsfaculty on 10/8/14.
//  Copyright (c) 2014 ___Ju Shen___. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>


#include "CSP.h"

using namespace std;


struct Problem{
	string level;
	int probNumber;
	int data[81];

};

int main(int argc, const char * argv[]) {

	// Read data from file
	Problem testData[77];
	string line;
	ifstream file ("SudokuProbs.txt");
	for (int num = 0;num < 77 ;num++){
		Problem prob;
		prob.probNumber = num+1;
		getline(file,line);

		while (line == "" || line == "/n" || line ==" "){
			getline(file,line);
		}
		prob.level = line;
		for (int i = 0;i < 81 ; i++ ) {
			char c;
			file >> c;
			prob.data[i] = c - '0';
		}

		testData[num] = prob;

	}




	    CSP csp;
	    csp.setData(testData[1].data);
	    csp.printState();
	    csp.forwardChecking();
	    csp.backtrackNum = 0;
	    while(!csp.goalCheck(csp.cur_state)){
	    	cout << "FINDING" << endl;
	    	csp.forwardCheckingOrder();
	    }
	    cout << "DONE" << endl;
	    cout << "Backtracked: " << csp.backtrackNum << endl;
	    csp.printState();

    return 0;
}
