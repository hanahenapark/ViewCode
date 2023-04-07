#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <climits> //INT_MAX
using namespace std;
// solution with the minimum number of coins
// Bottom up
//  Time complexity: O(m size *Value).
//  Auxiliary space: O(V) because using extra space for array table
//  m is size of coins array (number of different coins)
int minCoins(int coins[], int size, int Value)
{
    // table[i] will be storing the minimum number of coins
    // required for i value. So table[V] will have result
    int table[Value + 1];

    // Base case (If given value V is 0)
    table[0] = 0;

    // Initialize all table values as Infinity
    for (int i = 1; i <= Value; i++)
        table[i] = INT_MAX;

    // Compute minimum coins required for all
    // values from 1 to Value
    for (int i = 1; i <= Value; i++) // 1-11
    {
        // cout<<"i"<<i<<endl;
        // int coins[] = {9, 6, 5, 1};
        //  Go through all coins smaller than value
        for (int j = 0; j < size; j++)
        {
            cout << "\ti:" << i << " j:" << j << " coins[j]=" << coins[j] << endl;
            for (int k = 0; k < Value + 1; k++)
            {
                if (table[k] == INT_MAX)
                {
                    cout << "MAX,";
                }
                else
                {
                    cout << table[k] << ",";
                }
            }
            cout << endl;
            if (coins[j] <= i) // if coin is less than value
            {
                // cout << "j:" << j << " <=" << i << endl;
                int sub_res = table[i - coins[j]]; // value -coin, take away coin
                cout << "\t\tsub_res:" << sub_res + 1 << "<" << table[i] << endl;
                if (sub_res != INT_MAX && sub_res + 1 < table[i]) // if requires less coins, replace
                    table[i] = sub_res + 1;
            }
        }
    }

    if (table[Value] == INT_MAX) // if cannot make
        return -1;

    return table[Value];
}

// Driver program to test above function
int main()
{
    int coins[] = {9, 6, 5, 1};
    int size = sizeof(coins) / sizeof(coins[0]);
    int Value = 11;
    cout << "Minimum coins required is " << minCoins(coins, size, Value);
    return 0;
}
