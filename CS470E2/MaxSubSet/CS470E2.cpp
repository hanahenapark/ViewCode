
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

// Function to return max sum such that
// no two elements are adjacent
int FindMaxSum(vector<int> arr, int n)
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

// Driver code
int main()
{
    vector<int> arr = {192, 30, 337, 105, 350};
    int N = arr.size();

    // Function call
    cout << FindMaxSum(arr, N);
    return 0;
}