#include <bits/stdc++.h>
#include <climits> //INT_MAX
using namespace std;

int minRemovals(vector<vector<int>> &ranges)
{

    int size = ranges.size(), rem = 0;

    if (size <= 1)
        return 0;

    // Sort by minimum starting point
    sort(ranges.begin(), ranges.end(),
         [](const vector<int> &a, const vector<int> &b)
         { return a[0] < b[0]; });

    int end = ranges[0][1];
    for (int i = 1; i < ranges.size(); i++)
    {

        // If the current starting point is less than
        // the previous interval's ending point
        // (ie. there is an overlap)
        if (ranges[i][0] < end)
        {
            // increase rem
            rem++;
            // Remove the interval
            // with the higher ending point
            end = min(ranges[i][1], end);
        }
        else
            end = ranges[i][1];
    }

    return rem;
}

// Driver code
int main()
{
    vector<vector<int>> input = {{19, 25},
                                 {10, 20},
                                 {16, 20}};
    cout << minRemovals(input) << endl;
}
