#include <stdio.h>
#include <stdlib.h>
#include <iostream>  // std::cout
#include <algorithm> // std::sort
#include <vector>
#include <cstring>
using namespace std;

static bool compare(vector<int> &a, vector<int> &b)
{
    return a[0] < b[0];
}

int eraseOverlapIntervals(vector<vector<int>> &arr)
{

    int n = arr.size();

    sort(arr.begin(), arr.end(), compare);

    int count = 0;

    int i = n - 1;

    while (i > 0)
    {
        int j = i - 1;

        while (j >= 0 && arr[j][1] > arr[i][0])
        {
            count++;

            j--;
        }

        i = j;
    }

    return count;
}
int main()
{
    vector<vector<int>> input = {{19, 25},
                                 {10, 20},
                                 {16, 20}};
    cout << eraseOverlapIntervals(input) << endl;

    return 0;
}