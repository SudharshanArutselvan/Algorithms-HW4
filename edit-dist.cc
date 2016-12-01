/**
 *  \file edit-dist.cc.
 *
 *  Sudharshan Arutselvan
 *  Student ID: 89979164
 *
 * 	Ramkumar Rajabaskaran
 *  Student ID: 85241493
 *
 *  This file contains code to compute the edit distance to change string x to string y
 *  at a minimum cost possible.
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string>
#include <cstring>
#include <fstream>
#include <assert.h>
#include "timer.c"
using namespace std;


/* ============================================================

    Operations done:
    Operation[i][j]=1 --> Right       Cost --> 0
    Operation[i][j]=2 --> Replace     Cost --> 4
    Operation[i][j]=3 --> Insert      Cost --> 3
    Operation[i][j]=4 --> Delete      Cost --> 2
    Operation[i][j]=5 --> final

 ============================================================ */
int m,n,d[1000][1000],operation[1000][1000];
string opStrings[]={"Initial","Right","Replace by ","Insert ","Delete ","Final"};

//Function to print and perform and operations in the strings.
void printEditDist(string x, string y, string fileName){
    int i = 0,j = 0,k = 0,a,opDone,costSoFar = 0,stepCost = 0,gapop,gapt,u,t;
    string z,temp;
    z=x;
    //File to write the output of the edit distance computed.
    fileName=fileName.substr(0,fileName.find("."))+"_output.txt";
    ofstream writeFile(fileName.c_str());
    if(!writeFile){
        fprintf(stderr, "The file could not be created!\n");
        return;
    }
    writeFile<<"String-1:\n"<<x<<"\n\nString-2:\n"<<y<<"\n";
    writeFile<<"\nOperation      | C | Total |  z \n---------------------------------------------------------------------\n";
    writeFile<<"Initial        | "<<stepCost<<" |     "<<costSoFar<<" | *"<<x<<"\n";

    //Loop to perform the operations that has to be performed on the string x to get string y.
    while (operation[i][j] != 5) {  //Stop at final operation
        gapop=gapt=0;
        opDone = operation[i][j];
        switch(opDone) {
            case 1:   // Operation right
                z[k] = x[i];
                i++;
                j++;
                k++;
                stepCost = 0;
                writeFile<<opStrings[opDone];
                temp=opStrings[opDone];
                gapop=14-temp.length();
                break;
            case 2:   // Operation replace
                writeFile<<opStrings[opDone]<<"'"<<y[j]<<"'";
                z[k] = y[j];
                i++;
                j++;
                k++;
                stepCost = 4;
                break;
            case 3:   // Operation insert
                writeFile<<opStrings[opDone]<<"'"<<y[j]<<"'";
                z[k] = y[j];
                j++;
                k++;
                stepCost = 3;
                temp=opStrings[opDone];
                gapop=11-temp.length();
                break;    
            case 4:   // Operation delete
                writeFile<<opStrings[opDone]<<"'"<<x[i]<<"'";
                i++;
                stepCost = 2;
                temp=opStrings[opDone];
                gapop=11-temp.length();
                break;
            default:  
                cout<<"ERROR.\n";
                exit(1);
                break;
        }
        
        costSoFar+=stepCost;
        u=costSoFar;
        for(t=0;u>0;t++){
            u=u/10;
        }
        gapt=5-t;
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

// Function to get the inputs from the text file and find the operation that is done in minimum cost
void edit_dist(string fileName){
    string temp,x,y;
    fstream readFile;
    readFile.open(fileName.c_str());
    if(!readFile){
        fprintf(stderr, "The file could not be accessed!\n");
        return;
    }
    while(!readFile.eof()){ 
        getline(readFile,temp);     // Read a line from the input file
        m=atoi(temp.c_str());
        getline(readFile,x);
        getline(readFile,temp);
        n=atoi(temp.c_str());
        getline(readFile,y);
        getline(readFile,temp);
    }
    if(m!=x.length() || n!=y.length()){
        cout<<"Error in input file\n";
        exit(1);
    }
    readFile.close();
    int i,j,opDone,minVal;
    
    //Inititalize the d at m,n to be 0 cost and operation m,n to be final(5)
    d[m][n]=0;
    operation[m][n]=5;
    
    // The cost for deleting all characters from i to m if string y is empty at i
    for (i = 0; i < m; i++) {
        d[i][n] = 2*(m-i);
        operation[i][n] = 4;
    }

    // The cost for inserting all characters from j to n if string x is empty at j
    for (j = 0; j < n; j++) {
        d[m][j] = 3*(n-j);
        operation[m][j] = 3;
    }
    // Loop to find the minimum cost operation at i,j
    for (i = m-1; i >=0; i--) {
        for (j = n-1; j >= 0; j--) {
            if (x[i] == y[j]){
                opDone = 1;   // Operation right
                minVal=d[i+1][j+1] + 0;
            }
            else {
                opDone = 2;   // Operation replace
                minVal=d[i+1][j+1] + 4;
            }

            if (minVal > (d[i][j+1] + 3)) {
                minVal = d[i][j+1] + 3;
                opDone = 3;   // Operation insert
            }
              
            if (minVal > (d[i+1][j] + 2)) {
                minVal = d[i+1][j] + 2;
                opDone = 4;   // Operation delete
            }

            d[i][j] = minVal;
            operation[i][j] = opDone;
            //cout<<"Cost:"<<d[i][j]<<"  OPeration:"<<operation[i][j]<<"\n";
        }
    }

    //Function to print the edit distance
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
        writeFile<<"String-1:\n"<<x<<"\n\nString-2:\n"<<y<<"\n\nEdit Distance:"<<d[0][0]<<"\n";
    }
    cout<<"Edit Distance is "<<d[0][0]<<"\n";
}

// Main function to get the arguments and run the timer.
int main (int argc, char* argv[])
{
  int N = -1;

  if (argc == 2) {
  } else {
    cout<<"Error in command please refer README.md file to execute the program";
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
