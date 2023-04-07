/* A Top-Down DP implementation of LCS problem */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

void printVect(char *X, char *Y, int m, int n, vector<vector<int>> &vect)
{
    cout << "    ";
    for (int i = 0; i < n; i++)
    {
        cout << Y[i] << " ";
        cout.width(2);
    }
    cout << endl;
    for (int i = 0; i < vect.size(); i++)
    {
        // cout << X[i] << " ";

        for (int j = 0; j < vect[i].size(); j++)
        {
            cout << vect[i][j] << " ";
            cout.width(2);
        }
        if (i != 0)
        {
            cout << X[i - 1] << endl;
        }
        else
        {
            cout << endl;
        }
    }
}
/* Returns length of LCS for X[0..m-1], Y[0..n-1] */
// if back of x and y match, jump diagonal
// check if new value matches with above or left, move to matching (recursive)

int lcs(char *X, char *Y, int m, int n, vector<vector<int>> &dp)
{
    if (m == 0 || n == 0)
        return 0;
    if (X[m - 1] == Y[n - 1])
    {
        //  cout << "test:" << X[m - 1] << endl;
        return dp[m][n] = 1 + lcs(X, Y, m - 1, n - 1, dp);
    }
    if (dp[m][n] != -1)
    {

        return dp[m][n];
    }
    return dp[m][n] = max(lcs(X, Y, m, n - 1, dp),
                          lcs(X, Y, m - 1, n, dp));
}

/* Driver code */
int main()
{
    char X[] = "AGGTAB";
    char Y[] = "GXTXAYB";

    int m = strlen(X);
    int n = strlen(Y);
    // strlen X =num of vectors+1
    // strlen Y = lenght of vectors+1, all initalized to -1
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));

    cout << "Length of LCS is " << lcs(X, Y, m, n, dp) << endl;
    printVect(X, Y, m, n, dp);

    return 0;
}
