/* Dynamic Programming implementation of LCS problem */
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

// void printArray(int arr[][7], int RowSize, int ColSize)
// {
//     for (int x = 0; x < RowSize; x++)
//     {
//         for (int y = 0; y < ColSize; y++)
//         {
//             // cout << &arr[x][y] << " ";
//         }
//         cout << endl;
//     }
// }
/* Returns length of LCS for X[0..m-1], Y[0..n-1] */
void lcs(char *X, char *Y, int m, int n)
{
    int L[m + 1][n + 1];

    /* Following steps build L[m+1][n+1] in bottom up
    fashion. Note that L[i][j] contains length of LCS of
    X[0..i-1] and Y[0..j-1] */
    for (int i = 0; i <= m; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            if (i == 0 || j == 0)
            {
                L[i][j] = 0;
            }
            else if (X[i - 1] == Y[j - 1])
            {
                L[i][j] = L[i - 1][j - 1] + 1;
            }
            else
            {
                L[i][j] = max(L[i - 1][j], L[i][j - 1]);
            }
        }
    }

    for (int x = 0; x < m + 1; x++)
    {
        // cout << X[x] << " ";
        for (int y = 0; y < n + 1; y++)
        {
            cout << L[x][y] << " ";
        }
        cout << endl;
    }

    // Following code is used to print LCS
    int index = L[m][n];

    // Create a character array to store the lcs string
    char lcs[index + 1];
    lcs[index] = '\0'; // Set the terminating character

    // Start from the right-most-bottom-most corner and
    // one by one store characters in lcs[]
    int i = m, j = n;
    while (i > 0 && j > 0)
    {
        // If current character in X[] and Y are same, then
        // current character is part of LCS
        //===if top and left are equal go diagonal left and up
        if (X[i - 1] == Y[j - 1])
        {
            lcs[index - 1] = X[i - 1]; // Put current character in result, index filling in backwards
            i--;
            j--;
            index--; // reduce values of i, j and index
        }
        //====if not equal, go to largest of up or left
        // If not same, then find the larger of two and
        // go in the direction of larger value
        else if (L[i - 1][j] > L[i][j - 1])
            i--;
        else
            j--;
    }

    // Print the lcs
    cout << "LCS of " << X << " and " << Y << " is " << lcs;
}

/* Driver program to test above function */
int main()
{
    char X[] = "AGGTAB";
    char Y[] = "GXTXAYB";
    int m = strlen(X);
    int n = strlen(Y);
    lcs(X, Y, m, n);
    return 0;
}
