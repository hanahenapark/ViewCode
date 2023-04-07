//C++ code to maintain Prims Minimum Spanning Tree (MST) using Adjacency List
//Once Prims MST is determined an Vector of Vectors is 
//used to keep track of actual node connections after MST is determined
//weights=distance 
//function dfs goes through Vector of Vectors to print connections in Pre-Order 

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

//using pre-order to display final matrix
void dfs(int start, vector<bool>& visited, vector<vector<double>> finalMat) //o(V+E)
{
 
    std::cout << (char)(start+65)<< " ";

    visited[start] = true;
 
  
    for (int i = 0; i < finalMat[start].size(); i++) {
        if (finalMat[start][i] == 1 && (!visited[i])) {
            dfs(i, visited,finalMat);
        }
    }
}

double distance(int x1, int y1, int x2, int y2){ //O(1)
    //sqrt((x2-x1)^2+(y2-y1)^2)
    int x=pow(x2-x1,2);
    int y=pow(y2-y1,2);
    return sqrt(x+y);
}

struct AdjListNode { //O(1)
	int dest;
	double weight;
	struct AdjListNode* next;
};

struct AdjList { //O(1)
	struct AdjListNode* head; // pointer to head node of list
};

struct Graph { //O(1)
	int V;
	struct AdjList* array;
};

// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, double weight) //O(1)
{
	struct AdjListNode* newNode= (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->weight = weight;
	newNode->next = NULL;
	return newNode;
}

// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V) //O(V)
{
	struct Graph* graph= (struct Graph*)malloc(sizeof(struct Graph));
	graph->V = V;

	// Create an array of adjacency lists. Size of array
	// will be V
	graph->array = (struct AdjList*)malloc(
		V * sizeof(struct AdjList));

	// Initialize each adjacency list as empty by making
	// head as NULL
	for (int i = 0; i < V; ++i)
		graph->array[i].head = NULL;

	return graph;
}

// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest, double weight) //O(1)
{
	struct AdjListNode* newNode = newAdjListNode(dest, weight);
	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;

	// Since graph is undirected, add an edge from dest to
	// src also
	newNode = newAdjListNode(src, weight);
	newNode->next = graph->array[dest].head;
	graph->array[dest].head = newNode;
}

// Structure to represent a min heap node
struct MinHeapNode { //O(1)
	int v; 
	int key;
};

// Structure to represent a min heap
struct MinHeap { //O(1)
	int size; // Number of heap nodes present currently
	int capacity; // Capacity of min heap
	int* pos; // This is needed for decreaseKey()
	struct MinHeapNode** array;
};

// A utility function to create a new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, int key) //O(1)
{
	struct MinHeapNode* minHeapNode= (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
	minHeapNode->v = v;
	minHeapNode->key = key;
	return minHeapNode;
}

// A utility function to create a Min Heap
struct MinHeap* createMinHeap(int capacity) //O(1)
{
	struct MinHeap* minHeap= (struct MinHeap*)malloc(sizeof(struct MinHeap));
	minHeap->pos = (int*)malloc(capacity * sizeof(int));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array = (struct MinHeapNode**)malloc(capacity * sizeof(struct MinHeapNode*));
	return minHeap;
}

// A utility function to swap two nodes of min heap. Needed
// for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) //O(1)
{
	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

// A standard function to heapify at given idx
// This function also updates position of nodes when they
// are swapped. Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap, int idx) //O(logn)
{
	int smallest, left, right;
	smallest = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;

	if (left < minHeap->size&& minHeap->array[left]->key < minHeap->array[smallest]->key)
		smallest = left;

	if (right < minHeap->size && minHeap->array[right]->key < minHeap->array[smallest]->key)
		smallest = right;

	if (smallest != idx) {
		// The nodes to be swapped in min heap
		MinHeapNode* smallestNode = minHeap->array[smallest];
		MinHeapNode* idxNode = minHeap->array[idx];

		// Swap positions
		minHeap->pos[smallestNode->v] = idx;
		minHeap->pos[idxNode->v] = smallest;

		// Swap nodes
		swapMinHeapNode(&minHeap->array[smallest],
						&minHeap->array[idx]);

		minHeapify(minHeap, smallest);
	}
}

// A utility function to check if the given minHeap is empty
// or not
int isEmpty(struct MinHeap* minHeap) //O(1)
{
	return minHeap->size == 0;
}

// Standard function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap) //o(logn)
{
	if (isEmpty(minHeap))
		return NULL;

	// Store the root node
	struct MinHeapNode* root = minHeap->array[0];

	// Replace root node with last node
	struct MinHeapNode* lastNode= minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	// Update position of last node
	minHeap->pos[root->v] = minHeap->size - 1;
	minHeap->pos[lastNode->v] = 0;

	// Reduce heap size and heapify root
	--minHeap->size;
	minHeapify(minHeap, 0);

	return root;
}

// Function to decrease key value of a given vertex v. This
// function uses pos[] of min heap to get the current index
// of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, int key) //O(logn)
{
	// Get the index of v in heap array
	int i = minHeap->pos[v];

	// Get the node and update its key value
	minHeap->array[i]->key = key;

	// Travel up while the complete tree is not heapified.
	// This is a O(Logn) loop
	while (i&& minHeap->array[i]->key< minHeap->array[(i - 1) / 2]->key) {
		// Swap this node with its parent
		minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
		minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
		swapMinHeapNode(&minHeap->array[i],
						&minHeap->array[(i - 1) / 2]);

		// move to parent index
		i = (i - 1) / 2;
	}
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
bool isInMinHeap(struct MinHeap* minHeap, int v) //O(1)
{
	if (minHeap->pos[v] < minHeap->size)
		return true;
	return false;
}

// A utility function used to print the constructed MST
void printArr(int arr[], int n) //O(V)
{
	for (int i = 1; i < n; ++i)
		printf("%d - %d\n", arr[i], i);
}

// The main function that constructs Minimum Spanning Tree
// (MST) using Prim's algorithm
void PrimMST(struct Graph* graph, vector<vector<double>>& finalMat)
{
	int V = graph->V; // Get the number of vertices in graph
	int parent[V]; // Array to store constructed MST
	int key[V]; // Key values used to pick minimum weight
				// edge in cut

	// minHeap represents set E
	struct MinHeap* minHeap = createMinHeap(V);//O(1)

	// Initialize min heap with all vertices. Key value of
	// all vertices (except 0th vertex) is initially
	// infinite
	for (int v = 1; v < V; ++v) { //O(V)
		parent[v] = -1;
		key[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v, key[v]);  
		minHeap->pos[v] = v;
	}

	// Make key value of 0th vertex as 0 so that it
	// is extracted first
	
	key[0] = 0;
	minHeap->array[0] = newMinHeapNode(0, key[0]);	//O(N)
	minHeap->pos[0] = 0;

	// Initially size of min heap is equal to V
	minHeap->size = V;

	// In the following loop, min heap contains all nodes
	// not yet added to MST.
	while (!isEmpty(minHeap)) {												//O(V)
		// Extract the vertex with minimum key value
		struct MinHeapNode* minHeapNode= extractMin(minHeap);             //O(logn)
		int u= minHeapNode->v; // Store the extracted vertex number
		struct AdjListNode* pCrawl = graph->array[u].head;				//starting with Min
		while (pCrawl != NULL) {										//O(E) to traverse all adjacent vertices 
			int v = pCrawl->dest;

			// If v is not yet included in MST and weight of
			// u-v is less than key value of v, then update
			// key value and parent of v
			if (isInMinHeap(minHeap, v) && pCrawl->weight < key[v]) {
				key[v] = pCrawl->weight;
				parent[v] = u;
				decreaseKey(minHeap, v, key[v]);						//O(logn)
			}
			pCrawl = pCrawl->next;
		}
	}


	printArr(parent, V);
	//adding adges to final matrix that determines nodes 
	for (int i = 1; i < V; ++i){									//O(V^2)
	finalMat[parent[i]][i] = 1;
    finalMat[i][parent[i]] = 1;}
}


// Driver program to test above functions
int main()
{
	// Let us create the graph given in above figure
	int V = 8;
	struct Graph* graph = createGraph(V);
    //graph, source, dest, weight
    //weight = distance bewteen source and dest
    //every node is connect to every other
    //Given V={a: (1,3), b:(1,1),c: (0,0), d: (3,3), e: (4,2), f: (3,1), g: (5,1)}, h{2,-1}
   //x = givenPoints[a,b,c][x][y]
    vector<vector<double>> givenPoints{{0,1,3},//a
                                        {1,1,1},//b
                                        {2,0,0},//c
                                        {3,3,3},//d
                                        {4,4,2},//e
                                        {5,3,1},//f
                                        {6,5,1},//g
										{7,2,-1}//h
                                        };
	//O(V^2)
    for(int i=0;i<V;i++){
        for(int j=0;j<V;j++){
               // cout<<"givenPoints:("<<i<<")("<<j<<"):"<<distance(givenPoints[i][1],givenPoints[i][2], givenPoints[j][1], givenPoints[j][2])<<endl;
                addEdge(graph, i, j, distance(givenPoints[i][1],givenPoints[i][2], givenPoints[j][1], givenPoints[j][2]));
        }
    }
	vector<vector<double>> Mat=vector<vector<double> >(V, vector<double>(V, 0));
	PrimMST(graph, Mat);
	vector<bool> visited(V, false);
	dfs(0, visited,Mat);
	

	return 0;
}
