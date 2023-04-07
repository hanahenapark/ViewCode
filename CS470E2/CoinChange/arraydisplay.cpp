#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;
int main()
{
    int size = 5;
    int num = 3;
    // int table[num + 1][size];
    // for (int i = 0; i < num; i++)
    // {
    //     table[0][i] = 1;
    // }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < num; j++)
        {
            cout << "[" << i << "," << j << "]";
        }
        cout << endl;
    }
    return 0;
}