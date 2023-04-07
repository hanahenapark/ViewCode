#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Bottom Up Memoization
// Create 2D vector to store overlaping sub-problems
// Traverse whole array to find the solution
// store in memoization table
// use mem table to find opt. soln.

/*Hana Summary
going through a 2d array of coin possibilities (y) and sum (x)
add the following:
total count = 1. corresponding value above if can take + 2. value before
1. cooresponding value above is what you would be left with if you can take coin x out
2. value before is what you would add once everything is taken
*/
// Time Complexity: O(M*sum)
// Auxiliary Space: O(M*sum)
using namespace std;
int count(int coins[], int n, int sum)
{
    int i, j, x, y;

    // We need sum+1 rows as the table
    // is constructed in bottom up
    // manner using the base case 0
    // value case (sum = 0)
    // table[5][size=3]
    /*
    [0,0][0,1][0,2]
    [1,0][1,1][1,2]
    [2,0][2,1][2,2]
    [3,0][3,1][3,2]
    [4,0][4,1][4,2]*/

    int table[sum + 1][n];

    // Fill the entries for 0
    // value case (sum = 0)
    /*
    [1][1][1]
    [1,0][1,1][1,2]
    [2,0][2,1][2,2]
    [3,0][3,1][3,2]
     [4,0][4,1][4,2]
     */

    for (i = 0; i < n; i++)
        table[0][i] = 1;

    // Fill rest of the table entries
    // in bottom up manner
    for (i = 1; i < sum + 1; i++) // 1-4, skip first row
    {
        for (j = 0; j < n; j++) // 0-2
        {
            // Count of solutions including coins[j]
            x = (i - coins[j] >= 0) ? table[i - coins[j]][j]
                                    : 0;

            /*
            //(1,0) looks at 0,0,(1,1) leads to 0,
            //coins=1,2,3
            //for every row index (sum) - coin (every) >= 0
            //x=look at row-coin index=left over coin
            //if you can take away coins, take it away by looking at value left over (above) after taking it away
            if(i - coins[j] >= 0){
                x=table[i - coins[j]][j]
            }
            else{x=0}
            */

            // Count of solutions excluding coins[j]
            //
            y = (j >= 1) ? table[i][j - 1] : 0;
            /*
            // if not first colomn, get value before
            //if first colomn, add 0;

            if(j >= 1){
                y=table[i][j - 1]
            }
            else{y=0}
            */
            // total count = corresponding value above if can take + value before
            //  total count

            table[i][j] = x + y;
            // cout << "[" << x << "," << y << "]";
            cout << "[" << x + y << "]";
        }
        cout << endl;
    }
    // gets bottom corner
    return table[sum][n - 1];
}

int main()
{
    int coins[] = {1, 2, 3};
    int n = sizeof(coins) / sizeof(coins[0]);
    int sum = 4;
    cout << count(coins, n, sum);
    return 0;
}
