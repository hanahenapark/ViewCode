// 
#include <iostream>
#include <ostream>
#include <string>
using namespace std;
// captures the longest prefix which is also a suffix for every substring
int LPS(string s, int size)
{
	int n = size;

	int lps[n];
	lps[0] = 0; // lps[0] is always 0

	int len = 0;

	int i = 1;
	while (i < n)
	{
		if (s[i] == s[len])
		{
			len++;
			lps[i] = len;
			i++;
		}
		else 
		{
			if (len != 0)
			{
				len = lps[len-1];
			}
			else // if (len == 0)
			{
				lps[i] = 0;
				i++;
			}
		}
	}

	int res = lps[n-1];


	return res;
}

void printOutput(string s, int n){
    
    cout<<"OUTPUT:\"";
    for(int i=0;i<n;i++){
        cout<<s[i];
    }
    cout<<"\"";
}

int main()
{
	string s = "leetcodeleet";
	int num_matching= LPS(s, s.length());
    //cout<<num_matching<<endl;
    printOutput(s, num_matching);
	return 0;
}
