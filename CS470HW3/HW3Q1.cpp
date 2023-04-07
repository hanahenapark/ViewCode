// Hana Park KMP 
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

// Fills lps[] for given pattern pattern[0..M-1]
void computeLPSArray(char* pattern, int M, vector<int> &lps)
{
	// length of the previous longest prefix suffix
	int len = 0;

	lps[0] = 0; // lps[0] is always 0

	// the loop calculates lps[i] for i = 1 to M-1
	int i = 1;
	while (i < M) {
         
		if (pattern[i] == pattern[len]) {
			len++;
			lps[i] = len;
			i++;
		}
		else // (pattern[i] != pattern[len])
		{
			if (len != 0) {
				len = lps[len - 1];

				// Also, note that we do not increment
				// i here
			}
			else // if (len == 0)
			{
				lps[i] = 0;
				i++;
			}
		}
	}
    // for(int i=0;i<M;i++){
    // cout<<i<<":"<<lps[i]<<endl;
    // }
}

// Prints occurrences of txt[] in pattern[]
void KMP(char* pattern, char* txt)
{
	int M = strlen(pattern);
	int N = strlen(txt);

	// create lps[] that will hold the longest prefix suffix
	// values for patterntern
	//int lps[M];
    vector <int> lps(M,0);

	// Preprocess the patterntern (calculate lps[] array)
	computeLPSArray(pattern, M, lps);

	int i = 0; // index for txt[]
	int j = 0; // index for pattern[]

    //(N - i) >= (M - j)
	while ((N - i) >= (M - j)) {
		if (pattern[j] == txt[i]) {
			j++;
			i++;
		}

		if (j == M) {
	
			printf("Found pattern at index %d \n", i - j);
			j = lps[j - 1];
		}

		// mismatch after j matches
		else if (i < N && pattern[j] != txt[i]) {
			if (j != 0){
				j = lps[j - 1];}
			else{
				i = i + 1;
				}
		}
	}
}



// Driver program to test above function
int main()
{
    char txt1[] =   "aefcdfaecdaefaefcdaefeaefcdcdeae";
	char pattern1[]="aefcdaefeaefcd";
	cout<<"Test Case 1"<<endl;
    KMP(pattern1, txt1);

    char txt2[] = "aaabaabacdedfaabaabaaa";
	char pattern2[]="aabaabaaa";
	cout<<"Test Case 2"<<endl;
    KMP(pattern2, txt2);

    char txt3[] = "adafccfefbbbfeeccbcfd";
	char pattern3[]="ecb";
	cout<<"Test Case 3"<<endl;
    KMP(pattern3, txt3);

	return 0;
}
