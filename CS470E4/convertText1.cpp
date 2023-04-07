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
    inFile.open("testMatrix1.txt");
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