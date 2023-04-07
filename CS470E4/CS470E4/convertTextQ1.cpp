//Given form [[0,0],[2,2],[3, 10],[5,2],[7,0] ] in text file
//turns to {{0,0,0},{1,2,2},{2,3,10},{3,5,2},{4,7,0}};
//for Question 1
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
    int index=0;
    // Traverse the string
    for (int i = 0; i < N; i++) {
        
        
       
            if(!isspace(str[i])){
                if((str[i]=='[')||(str[i]=='('))
                {
                        cout<<"{";
                   
                }
                else if(str[i]==']'||(str[i]==')'))
                {
                    cout<<"}";
                }else {
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
    inFile.open("Q1Input.txt");
    int i = 0;
 
if (inFile.is_open()){
   
    //Read until no more lines in text file to read
    while (getline(inFile, instruction))
    {
    int N = instruction.length();
    
  
    TraverseString(instruction, N);
   cout<<";";
    cout<<endl;
   
    }
}
   
    return 0;
}