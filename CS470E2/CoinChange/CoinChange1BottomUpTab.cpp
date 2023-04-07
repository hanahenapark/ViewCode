#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
// Bottom up (Tabulation) with linear array
// Initialize with a linear array table with values equal to 0.
// With sum = 0, their is a way.
// Update the level wise number of ways of coin till the ith coin.
// Solve till j <= sum.
// Time Complexity: O(N*sum)
// Auxiliary Space: O(sum)

using namespace std;

// This code is
int count(int coins[], int n, int sum)
{
    // table[i] will be storing the number of solutions for
    // value i. We need sum+1 rows as the table is
    // constructed in bottom up manner using the base case (sum = 0)
    // table[4+1=5]

    /*Hana Summary
    going through sum array, each token indicates start indices
    from given token, incrementally go through until sum
    with that value add current value j to what happens when you take it away table[j-token]
    keep updating until end
    */
    int table[sum + 1];

    // Initialize all table values as 0
    memset(table, 0, sizeof(table));

    // Base case (If given value is 0)
    table[0] = 1;

    // Pick all coins one by one and update the table[]
    // values after the index greater than or equal to the
    // value of the picked coin
    //[1][0][0][0][0]
    //[1][0][0][0][0]
    // coins=1,2,3
    // 0-number of integers (3), 0-2

    for (int i = 0; i < n; i++)
    {                                         // 0-number of integers (3), 0-2
        for (int j = coins[i]; j <= sum; j++) // for every coin, until reach sum, coin-index start at 2->8, 4->8, 6->8
        {
            // cout << "i:" << i << " j:" << j << " table[" << j - coins[i] << "]=" << table[j - coins[i]] << endl;
            //  cout << "\ttable[" << j << "]=" << table[j] + table[j - coins[i]] << endl;

            table[j] += table[j - coins[i]]; // index=coin number, index- every coin amount
            // 2-first coin=0, 3-first coin(2) 0+
        }
        for (int j = 0; j <= sum; j++) // for every coin, until reach sum
        {
            cout << table[j] << " ";
        }
        cout << endl;
    }

    cout << endl;
    return table[sum]; // get last value
}

// Driver Code
int main()
{
    int coins[] = {2, 4, 6};
    int n = sizeof(coins) / sizeof(coins[0]);
    int sum = 8;
    cout << count(coins, n, sum);
    return 0;
}