static bool comp(vector<int a, vector<int> b>)
{
    return a[1] < b[1];
}
int findMinArrowShots(vector<vector<int>> &points)
{
    if (points.size() == 0)
        return 0;
    sort(points.begin(), points.end(), comp);
    int count = 1;
    int prev = points[0][1];
    for (int i = 0; i < points.size(); i++)
    {
        if (points[i][0] > prev)
        {
            prev = points[i][1];
            count++;
        }
    }
    return count;
