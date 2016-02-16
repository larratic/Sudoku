//
//  CSP.cpp
//  Sudoku
//
//  Created by cpsfaculty on 10/10/14.
//  Copyright (c) 2014 ___Ju Shen___. All rights reserved.
//

#include <stdio.h>

#include "CSP.h"
using namespace std;

/************************************** Below are the three functions you need to implement ***************************************/

/*Check whether current state satisfy the constraints*/
bool CSP::goalCheck(const State state)
{
 for(int x = 0; x < 9; x++)
    {
		for (int y = 0; y <9; y++)
		{

		if( variables[y][x].assignment == 0 || InRow(state,x,y,state.values[y][x]) || InCol(state,x,y,state.values[y][x]) ||  InBox(state,x,y,state.values[y][x]))
			return false;
	}
 }
	return true;
}

bool CSP::InRow(State state, int row,int pos, int num)
{

	 for (int col = 0; col < 9; col++)
	 {
		 if(col !=pos && variables[col][row].assignment != 0){
			if (variables[col][row].assignment == num)
			{
			    return true;
			}
		 }
	}
    return false;
}

bool CSP::InCol(State state,int pos, int col, int num)
{

	  for (int row = 0; row < 9; row++)
	  {
		  if (row !=pos && variables[col][row].assignment != 0)
		  {
			if (variables[col][row].assignment == num)
			{
            return true;
			}
		 }
	  }
    return false;
}

bool CSP::InBox(State state, int row1, int col1, int num)
{
	int boxRow = row1- row1%3;
	int boxCol = col1 - col1%3;
	 for (int row = 0; row < 3; row++)
	 {
        for (int col = 0; col < 3; col++)
		{
			if(row+boxRow !=row1 && col+boxCol !=col1 && variables[col][row].assignment != 0)
			{
			 if (variables[col+boxCol][row+boxRow].assignment == num)
				{
					return true;
				}
			}
		}
	 }
    return false;
}


/*Update Domain for the forward checking*/
void CSP::updateDomain(const State state)
{
 for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
			if(variables[y][x].assignment ==0)
			{
				variables[y][x].domain.clear();
				for(int i = 1; i <= 9; i++)
				 {
					variables[y][x].domain.push_back(i);
				}

				std::vector<int>::iterator it = variables[y][x].domain.begin();
				while( it != variables[y][x].domain.end())
					{

						if(InRow(state,x,y,*it)||InCol(state,x,y,*it)|| InBox(state,x,y,*it)) {
							
							variables[y][x].domain.erase(it);
							it = variables[y][x].domain.begin();
						}

						else
							it++;
					}
				}
			
		}
	
}



/*Arc consistency use*/
void CSP::arcConsistency(const State state)
{

	queue<Arrow> Q; // create queue
	// put all arcs into Q
	for(int x = 0; x < 9; x++) {
		for(int y = 0; y <9;y++){
	
			//Queue squares
			if(variables[y][x].assignment ==0)
			{
				int boxRow = x- x%3;
				int boxCol = y - y%3;
				for (int row = 0; row < 3; row++) {
					for (int col = 0; col < 3; col++){
						if(row+boxRow !=x && col+boxCol !=y ){
							Q.push(Arrow(x,y,row+boxRow, col+boxCol));
						}
					}
				}
			//Queue in row
				for (int col = 0; col < 9; col++){
					if(col !=y ){
						Q.push(Arrow(x,y,x,col));
					}
				}
			// Queue in column
				 for (int row = 0; row < 9; row++){
					 if (row !=x ){
						 Q.push(Arrow(x,y,row,y));
					}
				}
			}
		}
	
	} // Queue complete
	while(!Q.empty())
	{
		
		Arrow currArrow =  Q.front();
		Q.pop();
		int deleteSign = 0;
		 
		int irow = currArrow.headrow;
		int icol = currArrow.headcol;
		int jrow = currArrow.tailrow;
		int jcol = currArrow.tailcol;
	
		

		
		
		
		if(variables[icol][irow].assignment == 0 )
		{
		
		for(std::vector<int>::iterator itI = variables[icol][irow].domain.begin();itI != variables[icol][irow].domain.end();itI++)
			{
				
				bool i = false;
				if(variables[jcol][jrow].assignment == 0 )
				{
					
					for(std::vector<int>::iterator itJ = variables[jcol][jrow].domain.begin(); itJ != variables[jcol][jrow].domain.end(); itJ++)
					{
						
						if(*itI != *itJ)
						{
							i=true;
						}
						
					}
				
					if(!i)
					{
						variables[icol][irow].domain.erase(itI);	
						itI = variables[icol][irow].domain.begin();
						deleteSign = 1; 
					}
				}
		
			else {
				if(*itI == variables[jcol][jrow].assignment)
				{
				
					variables[icol][irow].domain.erase(itI);
					itI = variables[icol][irow].domain.begin();
					deleteSign = 1; 
					
				}
			}
		}
		
		}

	
		
		// if domain changed check neighbors 
		if(deleteSign ==1)
		{
			//Queue squares
			int boxRow = irow- irow%3;
			int boxCol = icol - icol%3;
			for (int row = 0; row < 3; row++) {
				for (int col = 0; col < 3; col++){
					if(row+boxRow !=irow && col+boxCol !=icol){
						Q.push(Arrow(irow,icol,row+boxRow, col+boxCol));
					}
				}
			}
			//Queue in row
			for (int col = 0; col < 9; col++){
				if(col !=icol ){
					Q.push(Arrow(irow,icol,irow,col));
				}
			}
			// Queue in column
			for (int row = 0; row < 9; row++){
				 if (row !=irow ){
					 Q.push(Arrow(irow,icol,row,icol));
					}
				}
			}
		}
	

	
}


void CSP::nakedRules(int k){
	vector<int> S;
	vector<int> sameDomain;
	bool foundNaked = 0;
	for (int col = 0;col++;col  < 9){ //check domains of each column
		int K = 0;
			sameDomain.clear();
			for (int a = 0;a++;a<9){
				for (int b=a+1;b++;b<9){
					S.clear();
					S.push_back(a);
					S.push_back(b);
					for (int row = 0;row++; row < 9){
						if(std::includes(S.begin(),S.end(),variables[col][row].domain.begin(),variables[col][row].domain.end())){
							foundNaked = 1;
							break;
						}
					}


				}
			}
	}

}

void CSP::printState()
{
  cout << "  1 2 3  4 5 6  7 8 9" << endl;
  cout << "--------------------"<< endl;
  for(int x = 0;x <9;x++){
    cout << x+1  ;

    for(int y = 0;y < 9; y++){
    	if (y%3 == 0) {
    	          	cout << "|";
    	          	}
      cout << variables[y][x].assignment << " ";

    }
    if ((x+1)%3 == 0){
           	cout <<endl<< "-----------------------";
           }


    cout << endl;
  }

}



/************************************************	End of Assignment ***********************************************/




CSP::CSP()
{
    /*Initially assign the domain, assignment for each variable and initialize the current state*/
    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 9; x++)
        {
            variables[y][x].assignment = 0; //Initialize the assignment
            
            /*Initialize the domain*/
            for(int i = 1; i <= 9; i++)
            {
                variables[y][x].domain.push_back(i);
            }

            
            cur_state.values[y][x] = 0; //Initizlize the current state
            
        }
    }
    
    alg_opt = 1; //initially set it as back track
    
    srand(time(NULL));
    random = 0;
}


CSP::~CSP()
{
    
}




void CSP::setData(int *data)
{
    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 9; x++)
        {
            int idx = x * 9 + y;
            variables[y][x].assignment = data[idx]; //Initialize the assignment
            cur_state.values[y][x] = data[idx]; //Initizlize the current state
            
        }
    }
}

void CSP::clearData()
{
    /*Initially assign the domain, assignment for each variable and initialize the current state*/
    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 9; x++)
        {
            variables[y][x].assignment = 0; //Initialize the assignment
            
            /*Initialize the domain*/
            variables[y][x].domain.clear();
            for(int i = 1; i <= 9; i++)
            {
                variables[y][x].domain.push_back(i);
            }
            
            cur_state.values[y][x] = 0; //Initizlize the current state
            
        }
    }
    
    /*Check whether a random domain is use*/
    if(random == 1)
        reshuffleDomain();
    
    repeating_list.clear();
    while(!assigned_variables.empty())
    {
        assigned_variables.pop();
        repeating_list.clear();
    }
    
}


void CSP::reshuffleDomain()
{
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        std::random_shuffle( variables[y][x].domain.begin(), variables[y][x].domain.end() );
    }
}

void CSP::sortDomain()
{
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        std::sort( variables[y][x].domain.begin(), variables[y][x].domain.end() );
    }
}

/*Cancel last assignment*/
int CSP::goBack()
{
    if(assigned_variables.size() > 0)
    {
        int cur_id = assigned_variables.top(); /*Remove last options*/
        assigned_variables.pop(); //pop out last option
        int y = cur_id / 9;
        int x = cur_id % 9;
        
        variables[y][x].assignment = 0; //assign the cell to zero
        cur_state.values[y][x] = 0; //update the assignment
        //*chosen_cell = cur_id;
        
       // printf("(%d, %d)\n", y, x);
        if(alg_opt == 2)
        {
            updateDomain(cur_state);
        }
        else if (alg_opt == 3)
        {
            arcConsistency(cur_state);
        }
        
    }
    
    return goalCheck(cur_state);
    
}


bool CSP::arcCheckingOrder()
{
    arcConsistency(cur_state);
    
    
    
    /*First go through all the variables and do backtrack if there is no empty domain */
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() == 0)
        {
            int available_assignemnt = 0; //an indicatior whether there are possible possible varaibles to be re-assigned
            while (available_assignemnt == 0) {
                int cur_id = assigned_variables.top();
                int y = cur_id / 9;
                int x = cur_id % 9;
                variables[y][x].assignment = 0;
                cur_state.values[y][x] = 0;
                arcConsistency(cur_state);
                
                
                for(int i = 0; i < variables[y][x].domain.size(); i++)
                {
                    State temp_state;
                    temp_state = cur_state;
                    temp_state.values[y][x] = variables[y][x].domain[i];
                    if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
                    {
                        cur_state = temp_state;
                        variables[y][x].assignment = variables[y][x].domain[i];
                        repeating_list.push_back(temp_state);
                        available_assignemnt = 1;
                        //*chosen_cell = cur_id;
                        arcConsistency(cur_state);
                        return false; //get out of the current varaible assignment
                    }
                }
                
                if(available_assignemnt == 0) //if all the domain values have been tried for current variable
                {
                    variables[y][x].assignment = 0;
                    cur_state.values[y][x] = 0;
                    assigned_variables.pop();
                    
                }
            }
            
        }
        
    }
    
    /*If there is no variable that has empty domain, then assign variable here*/
    /*First go through all the variables and do backtrack if there is no empty domain */
    int count = 0;
    while (count < 81)
    {
        /*Find the index of minimum number of domain*/
        int min_idx = 0;
        int min_num = 10; //because the maximum number of domain is 10
        for(int i = 0; i < 81; i++)
        {
            int y = i / 9;
            int x = i % 9;
            if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
            {
                if (variables[y][x].domain.size() < min_num) {
                    min_idx = i;
                    min_num = variables[y][x].domain.size();
                }
            }
        }
        
        int y = min_idx / 9;
        int x = min_idx % 9;
        
        /*If there is any varable has not been assigned yet, assign it and return it*/
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
        {
            /*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
            int id_min = 0;
            cur_state.values[y][x] = variables[y][x].domain[id_min];
            variables[y][x].assignment = variables[y][x].domain[id_min];
            assigned_variables.push(min_idx); //push the variable into stack, which will be used for backtrack (or DFS)
            repeating_list.push_back(cur_state); //make this state into the repeat_list
            //*chosen_cell = 9 * y + x;
            
            arcConsistency(cur_state); //Every time modify the assignment update the domain
            
            return false;
        }
        
        count++;
        
    }
    
    if(goalCheck(cur_state))
    {
        printf("find the goal\n");
        return true;
    }
	else
	{
		int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
		while (available_assignemnt == 0) {
			int cur_id = assigned_variables.top();
			int y = cur_id / 9;
			int x = cur_id % 9;
			variables[y][x].assignment = 0;
			cur_state.values[y][x] = 0;
			arcConsistency(cur_state);
			for(int i = 0; i < variables[y][x].domain.size(); i++)
			{
				State temp_state;
				temp_state = cur_state;
				temp_state.values[y][x] = variables[y][x].domain[i];
				if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
				{
					cur_state = temp_state;
					variables[y][x].assignment = variables[y][x].domain[i];
					repeating_list.push_back(cur_state);
					available_assignemnt = 1;
					//*chosen_cell = cur_id;
					break; //get out of the current varaible assignment
				}
			}

			if(available_assignemnt == 0) //if all the domain values have been tried for current variable
			{

				assigned_variables.pop();

			}
		}

		return false;
	}
    return false;
    
}



/*arcChecking without ordering*/
bool CSP::arcChecking()
{
    arcConsistency(cur_state);
    
    
    
    /*First go through all the variables and do backtrack if there is no empty domain */
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() == 0)
        {
            int available_assignemnt = 0; //an indicatior whether there are possible possible varaibles to be re-assigned
            while (available_assignemnt == 0) {
                int cur_id = assigned_variables.top();
                int y = cur_id / 9;
                int x = cur_id % 9;
                variables[y][x].assignment = 0;
                cur_state.values[y][x] = 0;
                arcConsistency(cur_state);
                
                
                for(int i = 0; i < variables[y][x].domain.size(); i++)
                {
                    State temp_state;
                    temp_state = cur_state;
                    temp_state.values[y][x] = variables[y][x].domain[i];
                    if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
                    {
                        cur_state = temp_state;
                        variables[y][x].assignment = variables[y][x].domain[i];
                        repeating_list.push_back(temp_state);
                        available_assignemnt = 1;
                        //*chosen_cell = cur_id;
                        arcConsistency(cur_state);
                        return false; //get out of the current varaible assignment
                    }
                }
                
                if(available_assignemnt == 0) //if all the domain values have been tried for current variable
                {
                    variables[y][x].assignment = 0;
                    cur_state.values[y][x] = 0;
                    assigned_variables.pop();
                    
                }
            }
            
        }
    }
    
    /*If there is no variable that has empty domain, then assign variable here*/
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        /*If there is any varable has not been assigned yet, assign it and return it*/
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
        {
            /*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
            int id_min = 0;
            cur_state.values[y][x] = variables[y][x].domain[id_min];
            variables[y][x].assignment = variables[y][x].domain[id_min];
            assigned_variables.push(i); //push the variable into stack, which will be used for backtrack (or DFS)
            repeating_list.push_back(cur_state); //make this state into the repeat_list
            //*chosen_cell = 9 * y + x;
            
            arcConsistency(cur_state); //Every time modify the assignment update the domain
            
            return false;
        }
        
    }
    
    if(goalCheck(cur_state))
    {
        printf("find the goal\n");
        return true;
    }
	else
	{
		int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
		while (available_assignemnt == 0) {
			int cur_id = assigned_variables.top();
			int y = cur_id / 9;
			int x = cur_id % 9;
			variables[y][x].assignment = 0;
			cur_state.values[y][x] = 0;
			arcConsistency(cur_state);
			for(int i = 0; i < variables[y][x].domain.size(); i++)
			{
				State temp_state;
				temp_state = cur_state;
				temp_state.values[y][x] = variables[y][x].domain[i];
				if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
				{
					cur_state = temp_state;
					variables[y][x].assignment = variables[y][x].domain[i];
					repeating_list.push_back(cur_state);
					available_assignemnt = 1;
					//*chosen_cell = cur_id;
					break; //get out of the current varaible assignment
				}
			}

			if(available_assignemnt == 0) //if all the domain values have been tried for current variable
			{

				assigned_variables.pop();

			}
		}

		return false;
	}
    return false;

}



/*Forward Checking algorithm*/
bool CSP::forwardChecking()
{
    updateDomain(cur_state); //the first step is based on current setting to update the domain

    
    /*First go through all the variables and do backtrack whether there is an empty domain */
    for(int i = 0; i < 81; i++)
    {

        int y = i / 9;
        int x = i % 9;

        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() == 0)
        {
            int available_assignemnt = 0; //an indicatior whether there are possible possible varaibles to be re-assigned
            while (available_assignemnt == 0) {
                int cur_id = assigned_variables.top();
                int y = cur_id / 9;
                int x = cur_id % 9;
                variables[y][x].assignment = 0;
                cur_state.values[y][x] = 0;
                updateDomain(cur_state);
                for(int i = 0; i < variables[y][x].domain.size(); i++)
                {
                    State temp_state;
                    temp_state = cur_state;
                    temp_state.values[y][x] = variables[y][x].domain[i];
                    if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
                    {
                        cur_state = temp_state;
                        variables[y][x].assignment = variables[y][x].domain[i];
                        repeating_list.push_back(temp_state);
                        available_assignemnt = 1;
                        //*chosen_cell = cur_id;
                        updateDomain(cur_state);
                        return false; //get out of the current varaible assignment
                    }
                }
                
                if(available_assignemnt == 0) //if all the domain values have been tried for current variable
                {
                    variables[y][x].assignment = 0;
                    cur_state.values[y][x] = 0;
                    assigned_variables.pop();
                    
                }
            }
            
        }
    }

    /*If there is no variable that has empty domain, then assign variable here*/
    for(int i = 0; i < 81; i++)
    {

        int y = i / 9;
        int x = i % 9;

        /*If there is any varable has not been assigned yet, assign it and return it*/
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
        {
            /*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
            int id_min = 0;
            cur_state.values[y][x] = variables[y][x].domain[id_min];
            variables[y][x].assignment = variables[y][x].domain[id_min];
            assigned_variables.push(i); //push the variable into stack, which will be used for backtrack (or DFS)
            repeating_list.push_back(cur_state); //make this state into the repeat_list
            //*chosen_cell = 9 * y + x;
            
            updateDomain(cur_state); //Every time modify the assignment update the domain
            return false;
        }
    }
    
    if(goalCheck(cur_state))
    {
        printf("found the goal\n");
        return true;
    }else
	{

		int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
		while (available_assignemnt == 0) {
			int cur_id = assigned_variables.top();
			int y = cur_id / 9;
			int x = cur_id % 9;
			variables[y][x].assignment = 0;
			cur_state.values[y][x] = 0;
			updateDomain(cur_state);
			for(int i = 0; i < variables[y][x].domain.size(); i++)
			{
				State temp_state;
				temp_state = cur_state;
				temp_state.values[y][x] = variables[y][x].domain[i];
				if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
				{
					cur_state = temp_state;
					variables[y][x].assignment = variables[y][x].domain[i];
					repeating_list.push_back(cur_state);
					available_assignemnt = 1;
					//*chosen_cell = cur_id;
					break; //get out of the current varaible assignment
				}
			}

			if(available_assignemnt == 0) //if all the domain values have been tried for current variable
			{

				assigned_variables.pop();

			}
		}

		return false;
	}
    return false;
    
}


/*Forward Checking algorithm*/
bool CSP::forwardCheckingOrder()
{

    updateDomain(cur_state); //the first step is based on current setting to update the domain
    
    

    /*First go through all the variables and do backtrack whether there is an empty domain */
    for(int i = 0; i < 81; i++)
    {

        int y = i / 9;
        int x = i % 9;
        
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() == 0)
        {
        	backtrackNum++;
            int available_assignemnt = 0; //an indicatior whether there are possible possible varaibles to be re-assigned
            while (available_assignemnt == 0) {
                int cur_id = assigned_variables.top();
                int y = cur_id / 9;
                int x = cur_id % 9;
                variables[y][x].assignment = 0;
                cur_state.values[y][x] = 0;
                updateDomain(cur_state);
                for(int i = 0; i < variables[y][x].domain.size(); i++)
                {
                    State temp_state;
                    temp_state = cur_state;
                    temp_state.values[y][x] = variables[y][x].domain[i];
                    if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
                    {
                        cur_state = temp_state;
                        variables[y][x].assignment = variables[y][x].domain[i];
                        repeating_list.push_back(temp_state);
                        available_assignemnt = 1;
                        //*chosen_cell = cur_id;
                        updateDomain(cur_state);
                        return false; //get out of the current varaible assignment
                    }
                }
                
                if(available_assignemnt == 0) //if all the domain values have been tried for current variable
                {

                    variables[y][x].assignment = 0;
                    cur_state.values[y][x] = 0;
                    assigned_variables.pop();
                    
                }
            }
            
        }
    }
    
    
    int count = 0;
    while (count < 81)
    {
        /*Find the index of minimum number of domain*/
        int min_idx = 0;
        int min_num = 10; //because the maximum number of domain is 10
        for(int i = 0; i < 81; i++)
        {
            int y = i / 9;
            int x = i % 9;
            if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
            {
                if (variables[y][x].domain.size() < min_num) {
                    min_idx = i;
                    min_num = variables[y][x].domain.size();
                }
            }
        }
        
        int y = min_idx / 9;
        int x = min_idx % 9;
        
        /*If there is any varable has not been assigned yet, assign it and return it*/
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
        {
            /*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
            int id_min = 0;
            cur_state.values[y][x] = variables[y][x].domain[id_min];
            variables[y][x].assignment = variables[y][x].domain[id_min];
            assigned_variables.push(min_idx); //push the variable into stack, which will be used for backtrack (or DFS)
            repeating_list.push_back(cur_state); //make this state into the repeat_list
            //*chosen_cell = 9 * y + x;
            
            updateDomain(cur_state); //Every time modify the assignment update the domain
            
            return false;
        }
        
        count++;
    }
    
    if(goalCheck(cur_state))
    {
        printf("found the goal\n");
        return true;
    }
	else
	{

		int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
		while (available_assignemnt == 0) {
			int cur_id = assigned_variables.top();
			int y = cur_id / 9;
			int x = cur_id % 9;
			variables[y][x].assignment = 0;
			cur_state.values[y][x] = 0;
			updateDomain(cur_state);
			for(int i = 0; i < variables[y][x].domain.size(); i++)
			{
				State temp_state;
				temp_state = cur_state;
				temp_state.values[y][x] = variables[y][x].domain[i];
				if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
				{
					cur_state = temp_state;
					variables[y][x].assignment = variables[y][x].domain[i];
					repeating_list.push_back(cur_state);
					available_assignemnt = 1;
					//*chosen_cell = cur_id;
					break; //get out of the current varaible assignment
				}
			}

			if(available_assignemnt == 0) //if all the domain values have been tried for current variable
			{

				assigned_variables.pop();

			}
		}

		return false;
	}

    return false;
    
}



/*Back Track to solve the problem*/
bool CSP::backTrack()
{
    
  
    
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        
        /*If there is any varable has not been assigned yet, assign it and break*/
        if(cur_state.values[y][x] == 0)
        {
            
            /*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
            int id_min = 0;
            cur_state.values[y][x] = variables[y][x].domain[id_min];
            variables[y][x].assignment = variables[y][x].domain[id_min];
            assigned_variables.push(i); //push the variable into stack, which will be used for backtrack (or DFS)
            repeating_list.push_back(cur_state); //make this state into the repeat_list
            //*chosen_cell = 9 * y + x;
            return false;
            
        }
    }
    
    /*If all the the variable are assigned*/
    {
        if(assigned_variables.size() == 0)//reset all the variables if there are no any varaibles assigned yet
        {
            for(int i = 0; i < 81; i++)
            {
                assigned_variables.push(i);
            }
        }
        
        if(goalCheck(cur_state))
        {
            printf("find the goal\n");
            return true;
        }
        else
        {
            int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
            while (available_assignemnt == 0) {
                int cur_id = assigned_variables.top();
                int y = cur_id / 9;
                int x = cur_id % 9;
                
                
                for(int i = 0; i < variables[y][x].domain.size(); i++)
                {
                    State temp_state;
                    temp_state = cur_state;
                    temp_state.values[y][x] = variables[y][x].domain[i];
                    if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
                    {
                        cur_state = temp_state;
                        variables[y][x].assignment = variables[y][x].domain[i];
                        repeating_list.push_back(cur_state);
                        available_assignemnt = 1;
                        //*chosen_cell = cur_id;
                        break; //get out of the current varaible assignment
                    }
                }
                
                if(available_assignemnt == 0) //if all the domain values have been tried for current variable
                {
                    variables[y][x].assignment = 0;
                    cur_state.values[y][x] = 0;
                    assigned_variables.pop();
                    
                }
            }
            
            return false;
        }
    }
}
