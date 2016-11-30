/**
 *  \file minimum-spanning-tree.cc.
 *  Sudharshan Arutselvan
 *  Student ID: 89979164
 * 	Ramkumar Rajabaskaran
 *  Student ID: 85241493
 *
 *  The file briefs the functions to create the adjacency list and the priority queue using heaps
 *  and also to create the minimum spanning tree using the Prim's algorithm.
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string>
#include <cstring>
#include <fstream>
#include <limits.h>
#include <float.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "timer.c"
using namespace std;


/* ============================================================
 */
int m,n,d[1000][1000],operation[1000][1000];
string opStrings[]={"Initial","Right","Replace","Insert","Delete","Final"};
void printEditDist(string x, string y, string fileName){
    int i = 0,j = 0,k = 0,a,opDone,costSoFar = 0,stepCost = 0,gapop,gapt,u,t;
    string z,temp;

    // Rather than implementing a string object for x that allows us
    //  constant-time inserts and deletes at the cursor, 
    //  we are just going to have two copies of the string.

    //  newString[0..k] stores everything before the cursor that we have
    //   changed, and x[i..m-1]  will store all the parts after the cursor that
    //    are still the same.

    z=x;
    fileName=fileName.substr(0,fileName.find("."))+"_output.txt";
    ofstream writeFile(fileName.c_str());
    if(!writeFile){
        fprintf(stderr, "The file could not be created!\n");
        return;
    }
    writeFile<<"\nOperation | C | Total |  z \n---------------------------------------------------------------------\nInitial   | "<<stepCost<<" |     "<<costSoFar<<" | *"<<x<<"\n";   
    while (operation[i][j] != 5) {

        opDone = operation[i][j];
        switch(opDone) {
            case 1:
                z[k] = x[i];
                i++;
                j++;
                stepCost = 0;
                break;
            case 2:
                z[k] = y[j];
                i++;
                j++;
                stepCost = 4;
                break;
            case 3:
                z[k] = y[j];
                j++;
                stepCost = 3;
                break;    
            case 4:
                i++;
                stepCost = 2;
                break;
            default:  // We should never a follow a pointer to initial or final.
                cout<<"ERROR.\n";
                exit(1);
                break;
        }

        if (opDone != 4) {
            k++;
        }
        temp=opStrings[opDone];
        gapop=9-temp.length();
        costSoFar += stepCost;
        u=costSoFar;
        for(t=0;u>0;t++){
            u=u/10;
        }
        gapt=5-t;
        writeFile<<opStrings[opDone];
        for(int g=0;g<gapop;g++){
            writeFile<<" ";
        }
        writeFile<<" | "<<stepCost<<" | ";
        for(int g=0;g<gapt;g++){
            writeFile<<" ";
        }
        writeFile<<costSoFar<<" | "<<z.substr(0,k)<<"*"<<x.substr(i,m)<<"\n";
    }
    writeFile<<"\nEdit Distance:"<<d[0][0]<<"\n";
}
void edit_dist(string fileName){
    string temp,x,y;
    fstream readFile;
    readFile.open(fileName.c_str());
    if(!readFile){
        fprintf(stderr, "The file could not be accessed!\n");
        return;
    }
    void init();       // Initialize the list and queue

    //Create the adjacency list
    while(!readFile.eof()){ 
        getline(readFile,temp);     // Read a line from the input file
        m=atoi(temp.c_str());
        getline(readFile,x);
        getline(readFile,temp);
        n=atoi(temp.c_str());
        getline(readFile,y);
        getline(readFile,temp);
    }
    readFile.close();
    int i,j,opDone;
    
    d[m][n]=0;
    operation[m][n]=5;
    
    for (i = 0; i < m; i++) {
        // To convert x[i..m-1] to the null string y[n], delete all (m-i)
        //   remaining characters in x.
        d[i][n] = 2*(m-i);
        operation[i][n] = 4;
    }

    for (j = 0; j < n; j++) {
        // To convert x[m] (the null string) into  y[j..n-1], insert
        //   the missing n-j characters.
        d[m][j] = 3*(n-j);
        operation[m][j] = 3;
    }
    // Start at d[m][n] and loop backwards
    for (i = m-1; i >=0; i--) {
        for (j = n-1; j >= 0; j--) {
            int costForReplaceOrMove;
            int costForInsert;
            int costForDelete;
            int minValue;

            // Compute d[i][j] as the minimum of 4 terms:

            // If x[i] == y[j], we could move right.
            //  Otherwise, we can replace x[i] with y[j] and
            //   increment i and j.
            costForReplaceOrMove = d[i+1][j+1] + 4*(x[i] != y[j]) + 0*(x[i] == y[j]);

            // If we insert a character into x to match y[j], then
            //   we increment j by 1
            costForInsert = d[i][j+1] + 3;

            // If we delete a character in x, then we
            //  increment i by 1.
            costForDelete = d[i+1][j] + 2;


            // Of the above operations, find one that gives us
            //  a minimum cost.

            minValue = costForReplaceOrMove;
            if (x[i] != y[j]){
                opDone = 2;
            }
            else {
                opDone = 1;
            }

            if (minValue > costForInsert) {
                minValue = costForInsert;
                opDone = 3;
            }
              
            if (minValue > costForDelete) {
                minValue = costForDelete;
                opDone = 4;
            }

            d[i][j] = minValue;
            operation[i][j] = opDone;
            //cout<<"Cost:"<<d[i][j]<<"  OPeration:"<<operation[i][j]<<"\n";
        }
    }
    if ((m < 75) && (n < 75)) {
        printEditDist(x, y, fileName);
    }
    else{
        fileName=fileName.substr(0,fileName.find("."))+"_output.txt";
        ofstream writeFile(fileName.c_str());
        if(!writeFile){
            fprintf(stderr, "The file could not be created!\n");
            return;
        }
        writeFile<<x<<"\n"<<y<<"\nEdit Distance:"<<d[0][0]<<"\n";
    }
    cout<<"Edit Distance is "<<d[0][0]<<"\n";
}

int main (int argc, char* argv[])
{
  int N = -1;

  if (argc == 2) {
  } else {
    cout<<"Wrong command";
    return -1;
  }
  stopwatch_init ();
  struct stopwatch_t* timer = stopwatch_create ();
  assert (timer);
  stopwatch_start (timer);
  edit_dist(string(argv[1]));
  long double stopTime= stopwatch_stop (timer);
  cout<<"Running Time:"<<stopTime<<"\n";
  return 0;
}
/* eof */
