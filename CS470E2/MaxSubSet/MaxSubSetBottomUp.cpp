// C++ code to implement the approach
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <vector>
using namespace std;
// maxsubset

// dp[Num of elements][2]
// ===excluded===>dp[i][0]=max(dp[i-1][0], dp[i-1][1]) ==>max of above row
//-first colomn holds max sub sequence till ith index

// with arr[i] excluded
// ==included====> dp[i][1]= dp[i-1][0] + arr[i]    ==>above + index number
//-second colon hold with arr[i] included

// If the size of the array is 1, then the answer is arr[0].
//  Initialize the values of dp[0][0] = 0 and dp[0][1] = arr[0].
//  Iterate from i = 1 to N-1:
//  Fill the dp array as per the relation shown above.
//  Return the maximum between dp[N-1][1] and dp[N-1][0] as that will be the answer

//  Time Complexity: O(N)
//  Auxiliary Space: O(N)
//   Function to find the maximum sum
int findMaxSum(vector<int> arr, int N)
{
    // Declare dp array
    int dp[N][2];
    if (N == 1)
    {
        return arr[0]; // first element of array
    }

    // Initialize the values in dp array, first row =[0][first elment]
    dp[0][0] = 0;
    dp[0][1] = arr[0];
    // cout << dp[0][0] << " " << dp[0][1] << endl;

    // Loop to find the maximum possible sum
    // vector<int> arr = {5, 5, 10, 100, 10, 5};
    for (int i = 1; i < N; i++)
    {
        dp[i][0] = max(dp[i - 1][1], dp[i - 1][0]); // max of above row (max of previous include or exclude), excluded
        dp[i][1] = dp[i - 1][0] + arr[i];           // add above and array element, included
                                                    //  cout << arr[i] << ":" << dp[i][0] << " " << dp[i][1] << endl;
    }

    // Return the maximum sum
    return max(dp[N - 1][0], dp[N - 1][1]);
}

// with space optimization
//  Function to return max sum such that
//  no two elements are adjacent
int FindMaxSumSpace(vector<int> arr, int n)
{
    int incl = arr[0];
    int excl = 0;
    int excl_new;
    int i;

    for (i = 1; i < n; i++)
    {
        // Current max excluding i
        excl_new = max(incl, excl);

        // Current max including i
        incl = excl + arr[i];
        excl = excl_new;
    }

    // Return max of incl and excl
    return max(incl, excl);
}

// Driver Code
int main()
{
    // Creating the array
    vector<int> arr = {192, 30, 337, 105, 350};
    int N = arr.size();

    // Function call
    cout << findMaxSum(arr, N) << endl;
    return 0;
}
