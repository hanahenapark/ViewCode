#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

// Top Down Memoization
//  Creating a 2-D vector to store the Overlapping Solutions
//  Keep Track of the overlapping subproblems
// while Traversing the array coins[]
//  Recall them whenever needed
// Similar to recursive except values are stores

using namespace std;

int coinchange(vector<int> &a, int v, int n,
               vector<vector<int>> &dp)
{
    if (v == 0)
        return dp[n][v] = 1;
    if (n == 0)
        return 0;
    if (dp[n][v] != -1)
        return dp[n][v];
    if (a[n - 1] <= v) // if value !=0, size!=0, and if not set (not -1)
    {
        // Either Pick this coin or not,
        return dp[n][v] = coinchange(a, v - a[n - 1], n, dp) + coinchange(a, v, n - 1, dp);
    }
    else // We have no option but to leave this coin
    {
        return dp[n][v] = coinchange(a, v, n - 1, dp);
    }
}
int main()
{
    int tc = 1;
    // cin >> tc;
    while (tc--) // evaluate to be true or false then decrement
    {
        // cout << tc << endl;
        vector<int> a = {1, 2, 3};

        int n = a.size(), valueSum = 6;
        // size+1 vectors of int vectors
        // int vectors have length of sum+1, set all to -1
        vector<vector<int>> dp(n + 1, vector<int>(valueSum + 1, -1));
        int result = coinchange(a, valueSum, n, dp);
        cout << result << endl;
    }
    return 0;
}