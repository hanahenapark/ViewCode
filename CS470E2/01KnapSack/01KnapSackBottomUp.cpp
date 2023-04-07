// A dynamic programming based
// solution for 0-1 Knapsack problem
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

// A utility function that returns
// maximum of two integers
// Time Complexity: O(N*W).
// where ‘N’ is the number of weight element and ‘W’ is capacity. As for every weight element we traverse through all weight capacities 1<=w<=W.
// Auxiliary Space: O(N*W).
// The use of 2-D array of size ‘N*W’.
int max(int a, int b)
{
    return (a > b) ? a : b;
}
void printVect(vector<vector<int>> &vect)
{
    for (int i = 0; i < vect.size(); i++)
    {
        // cout << X[i] << " ";
        cout << endl;
        for (int j = 0; j < vect[i].size(); j++)
        {
            cout << vect[i][j] << " ";
            cout.width(4);
        }
    }
}

// Returns the maximum value that
// can be put in a knapsack of capacity W
int knapSack(int W, int wt[], int val[], int n)
{
    int i, w;
    // size =y
    // weight=x
    vector<vector<int>> K(n + 1, vector<int>(W + 1));

    // Build table K[][] in bottom up manner
    for (i = 0; i <= n; i++)
    {
        for (w = 0; w <= W; w++)
        {
            if (i == 0 || w == 0)
            {
                cout << "-";
                K[i][w] = 0;
            }
            else if (wt[i - 1] <= w) // 10<=10
            {
                cout << "+"; // determine if weigh is possible to add
                             // if val+ up and total weight minus value weight
                             //  or weight above
                K[i][w] = max(val[i - 1] + K[i - 1][w - wt[i - 1]], K[i - 1][w]);
            }
            else
            {
                cout << "="; // looking above
                K[i][w] = K[i - 1][w];
            }
            cout << "K[" << i << "][" << w << "]=" << K[i][w] << endl;
        }
    }

    printVect(K);
    return K[n][W];
}

// Driver Code
int main()
{
    int val[] = {6, 10, 12};
    int wt[] = {1, 2, 3};
    int W = 5;
    int n = sizeof(val) / sizeof(val[0]);

    cout << knapSack(W, wt, val, n);

    return 0;
}

// This code is contributed by Debojyoti Mandal
