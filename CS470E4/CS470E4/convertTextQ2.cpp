//Given form below in text file
/*
0    4    0    0    0    0    0    5     0
4    0    8    0    0    0    0   11    0
0    8    0    7    0    4    0    0     2
0    0    7    0    9    14  0    0     0
0    0    0    9    0    5    0    0     0
0    0    4    14  5   0   2    0     0
0    0    0    0    0    2    0    10   3
5   11   0    0    0    0   10    0    7
0    0    2    0    0    0    3    7    0
]*/
/*
Return
{{0,4,0,0,0,0,0,5,0},
{4,0,8,0,0,0,0,11,0},
{0,8,0,7,0,4,0,0,2},
{0,0,7,0,9,14,0,0,0},
{0,0,0,9,0,5,0,0,0},
{0,0,4,14,5,0,2,0,0},
{0,0,0,0,0,2,0,10,3},
{5,11,0,0,0,0,10,0,7},
{0,0,2,0,0,0,3,7,0},
};
*/
//

//for Question 2
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <bitset>
#include <ctype.h>

using namespace std;
// Function to traverse the string and
// print the characters of the string
void TraverseString(string &str, int N)
{ 
    // Traverse the string
    for (int i = 0; i < N; i++) {
        
        if(i!=N-1){
            if(!isspace(str[i])){
            cout<<str[i];
            if(isdigit(str[i])&&(!isdigit(str[i+1]))){
                cout<<",";
            }
            }
            
        }
        else{
            if(isdigit(str[i])){
                cout<<str[i];
            }
        }
      
        // Print current character
       
    }
      
}
  
int main(){

  string instruction;
    vector<string> myString;
    ifstream inFile;
    inFile.open("Q2input.txt");
    int i = 0;
  cout<<"{";
if (inFile.is_open()){
   
    //Read until no more lines in text file to read
    while (getline(inFile, instruction))
    {
    int N = instruction.length();
     cout<<"{";
  
    TraverseString(instruction, N);
    cout<<"},";
    cout<<endl;
   
    }
}
   cout<<"};";
    return 0;
}