//*** Question 47. Please implement the source code of MST-PRIM algorithm. 
//The pseudocode is in the textbook (Cormen’s version), page 634.
// Test case example:
// Adjacency matrix of undirected graph G.
// A C++ program for Prim's Minimum
// Spanning Tree (MST) algorithm. The program is
// for adjacency matrix representation of the graph
//O (V^2)
#include <cstring>
#include <iostream>
#include <vector>
#include <stdbool.h>
#include <climits>
using namespace std;

// Number of vertices in the graph
#define V 9


int minKey(int key[], bool mstSet[])
{
	// Initialize min value
	int min = INT_MAX, min_index;

	for (int v = 0; v < V; v++)
		if (mstSet[v] == false && key[v] < min)
			min = key[v], min_index = v;

	return min_index;
}


void printMST(int parent[], int graph[V][V])
{   int final[V][V]={0};
	//cout << "Edge \tWeight\n";
	for (int i = 1; i < V; i++){
		//cout << parent[i] << " - " << i << " \t" << graph[i][parent[i]] << " \n";
        final[parent[i]][i]=graph[i][parent[i]];
        final[i][parent[i]]=graph[i][parent[i]];
    }

    for(int i=0;i<V;i++){
        for(int j=0;j<V;j++){
            cout<<final[i][j]<<" ";
        }
        cout<<endl;
    }
}


void primMST(int graph[V][V])
{
	int parent[V];

	int key[V];

	bool mstSet[V];

	for (int i = 0; i < V; i++)
		key[i] = INT_MAX, mstSet[i] = false;
	key[0] = 0;
	parent[0] = -1; 

	// The MST will have V vertices
	for (int count = 0; count < V - 1; count++) {
		int u = minKey(key, mstSet);
		mstSet[u] = true;
		for (int v = 0; v < V; v++)
			if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v]){
				parent[v] = u, key[v] = graph[u][v];
                }
	}

    // for(int i=0; i<V-1;i++){
    //     for (int v = 0; v < V; v++){
    //         cout<<graph[i][v]<<" ";
    //     }
    //     cout<<endl;
    // }   
	// print the constructed MST
	printMST(parent, graph);
}

// Driver's code
int main()
{
	int graph[V][V] = {{0,4,0,0,0,0,0,8,0},
{4,0,8,0,0,0,0,11,0},
{0,8,0,7,0,4,0,0,2},
{0,0,7,0,9,14,0,0,0},
{0,0,0,9,0,10,0,0,0},
{0,0,4,14,10,0,2,0,0},
{0,0,0,0,0,2,0,1,6},
{8,11,0,0,0,0,1,0,7},
{0,0,2,0,0,0,6,7,0},
};
    
    int len = sizeof(graph[0]) / sizeof(int);


	primMST(graph);

	return 0;
}


