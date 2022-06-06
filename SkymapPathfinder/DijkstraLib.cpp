//Code for pathfinding the shortest path on a map. Algorithm used: Dijkstra
// C++ program for Dijkstra's shortest path algorithm.
// The program transforms the matrix cloud in adjacency matrix of the relative graph.
/**
	Example:

		shortest path from: 0 -> 9  will be: 5 9        with distance weight of (5 + 2) = 7

		input map        SymbolTable map        path result                           Adjacency matrix:
		3 4

		5 2 3 8          0 1 2 3                (5). . .                             0 5 0 0 5 5 0 0 0 0 0 0
		4 2 7 1    -->   4 5 6 7          -->    . 2 . .                             2 0 2 0 2 2 2 0 0 0 0 0
		3 2 1 3          8 9 10 11               . 2 . .                             0 3 0 3 0 3 3 3 0 0 0 0
																					 0 0 8 0 0 0 8 8 0 0 0 0
		the result path use only the number of the SymbolTable map.                  4 4 0 0 0 4 0 0 4 4 0 0
		Diagonals are allowed.                                                       2 2 2 0 2 0 2 0 2 2 2 0
																					 0 7 7 7 0 7 0 7 0 7 7 7
																					 0 0 1 1 0 0 1 0 0 0 1 1
																					 0 0 0 0 3 3 0 0 0 3 0 0
																					 0 0 0 0 2 2 2 0 2 0 2 0
																					 0 0 0 0 0 1 1 1 0 1 0 1
																					 0 0 0 0 0 0 3 3 0 0 3 0
*/

#include <stdio.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "DijkstraLib.h"
#include "Point.h"

//#include "GraphDBG.h"
//#include "GraphADJ.h"

//DEBUG flag
#define DEBUG_DIJKSTRA 0		//use ONLY on small matrix -> view all possible path
#define DISPLAY 1				//display on video the path
#define DISPLAY_ADJ 0			//display adj matrix
#define DISPLAY_CLOUD 1			//display cloud map
#define DISPLAY_PATH 1			//display the path

using namespace std;

/**
Create the shortest path from the source - NOTE: the path is reverse
@Param1: parents vertices visited, best path
@Param2: destination position
@Param3: indexing used for storing values in path array
*/
void dijkstra::createShortestPath(int* parent, int j, int i) {
	if (parent[j] == -1) {
		return;
	}
	i++;
	createShortestPath(parent, parent[j], i);
	path[i] = j;
	//printf("j:%d, i:%d, path: %d\n", j, i, path[i]);
}

/**
Get the true shortest path after reversed it
@Return: the shortest path
*/
int* dijkstra::getShortestPath() {
	//reverse the path
	int length = 0;
	for (int i = 1; i < numVertices; i++) {
		if (path[i] == -1) {
			break;
		}
		length++;
	}
	lengthPath = length;

	//printf(" LENGHT: %d  ", lenght);
	int* realPath = (int*)malloc((length + 1) * sizeof(int)); //(int*)malloc((length + 1) * sizeof(int)); //new int[length + 1];
	int j = 0;
	for (int i = length; i > 0; i--) {
		realPath[j] = path[i];
		j++;
	}

	return realPath;
}
/**
Recursive function that print on video only the path
@Param: parents array, vertex previous visited
@Param: source vertex / previous vertex
*/
void dijkstra::printPath(int parent[], int j) {

	// Base Case : If j is source
	if (parent[j] == -1) {
		return;
	}

	printPath(parent, parent[j]);
	printf("%d ", j);
}

/**
Print all possible path from that source point
@Param1: source point
@Param2: distance array
@Param3: parents array, vertices previous visited
*/
void dijkstra::printDijkstra(int sourceVertex, int key[], int* parent) {

	printf("Result of all path\n");
	for (int i = 0; i < numVertices; i++) {
		printf("Source %d -> %d, distance: %d -- path %d ", sourceVertex, i, key[i], sourceVertex);
		printPath(parent, i);
		printf("\n");
	}
}

/**
Get the vertex with minimum distance which is not included in SPT
@Param1: minimum spanning tree array
@Param2: distance array
@Return: the minimum vertex
*/
int dijkstra::getMinimumVertex(bool mst[], int key[]) {
	int minKey = INT_MAX;
	int vertex = -1;
	for (int i = 0; i < numVertices; i++) {
		if (mst[i] == false && minKey >= key[i]) {
			minKey = key[i];
			vertex = i;
			//printf("minkey %d > key %d , mst[%d] bool: %d\n",minKey, key[i], i, mst[i]); //ELIMIANNNNNNNNNNNNNN
		}
	}
	return vertex;
}

/**
Function with Dijkstra Algorithm
@param1: adjacency matrix
@param2: source position
@param3: destination position
*/
void dijkstra::dijkstraAlgorithm(int** matrix, int sourceVer, int dest) {

	int* distance =  (int*)malloc((numVertices + 10) * sizeof(int)); //new int[numVertices];     //array of distance from every node
	int* parent = (int*)malloc((numVertices + 10) * sizeof(int)); //new int[numVertices];      //array with all previous node visit
	bool* spt =  (bool*)malloc((numVertices + 10) * sizeof(bool)); //new bool[numVertices];      //array of spanning tree visit

	sourceVertex = sourceVer;
	destinationVertex = dest;

	//Initialize all the distance to infinity
	for (int i = 0; i < numVertices + 5; i++) {
		parent[sourceVertex] = -1;
		distance[i] = INT_MAX;
		spt[i] = false;
		path[i] = -1;
	}
	//distance from the start vertex = 0
	distance[sourceVertex] = 0;

	//create SPT
	for (int i = 0; i < numVertices - 1; i++) {

		//get the vertex with the minimum distance
		int vertex_U = getMinimumVertex(spt, distance);

		//include this vertex in SPT
		spt[vertex_U] = true;

		//iterate through all the adjacent vertices of above vertex and update the keys
		for (int vertex_V = 0; vertex_V < numVertices; vertex_V++) {

			//check of the edge between vertex_U and vertex_V
			//check if this vertex 'vertex_V' already in spt and if distance[vertex_V]!=Infinity
			if (matrix[vertex_U][vertex_V] > 0 && spt[vertex_V] == false && matrix[vertex_U][vertex_V] != INT_MAX) {

				//check if distance needs an update or not means check total weight from source to vertex_V is less than
				//the current distance value, if yes then update the distance

				int newKey = matrix[vertex_U][vertex_V] + distance[vertex_U];
				if (newKey < distance[vertex_V]) {
					distance[vertex_V] = newKey;
					parent[vertex_V] = vertex_U;
				}
			}
		}
	}

	for (int t = 0; t < numVertices; t++) {
		if (t == dest) {
			//printf("Weight: %d\n", distance[t] + getValueMatrix(destinationVertex));
			costPath = distance[t] + getValueMatrix(destinationVertex);
			createShortestPath(parent, t, 0);
			break;
		}
	}

	//print shortest path
	if (DEBUG_DIJKSTRA) {
		printDijkstra(sourceVertex, distance, parent);
	}

}

/**
Retrieve the matrix index position of a point on map -> transformation of map in SymbolTable matrix
@param1 and @param2: by reference is passed the position on map
@param3 is the position to be searched
@param4 is the weather matrix
*/
void dijkstra::retrieve_coordinate(int* i, int* j, int p, int** m) {
	/*i = p / (height);

	//if (p <= width) {
	//    *j = p-1;
	//} else {
	// *j = p % (width);
	//}

	*j = p - *i*height;{}
	printf("\t\t\t i:%d j:%d - p:%d h:%d w:%d\n", *i, *j, p, height, width);*/

	//** FIND A BETTER SOLUTION FOR THIS FUNCTION **

	int cnt = 0;
	for (int s = 0; s < rows; s++) {
		for (int g = 0; g < cols; g++) {
			cnt++;
			if (cnt == p) {
				*i = s;
				*j = g;
				return;
			}
		}
	}
}

/**
Create the Adjacency matrix (diagonal is always 0) from the map matrix
@Param1: cloud map
@Param2: height of cloud map
@Param3: width of cloud map
@Return: the weighted adjacency matrix (the graph)
*/
int** dijkstra::createGraph(int** cloudMap) {

	int** graph = (int**)malloc((numVertices + 1) * sizeof(int*)); //new int*[numVertices + 1];
	for (int i = 0; i < (numVertices + 1); i++)
		graph[i] = (int*)malloc((numVertices + 1) * sizeof(int)); // new int[numVertices + 1];

	cloudPositionMap = new int*[rows];
	for (int i = 0; i < rows; i++)
		cloudPositionMap[i] = new int[cols];
	cloudPositionMap = cloudMap;

	//reset to zero the graph
	for (int i = 0; i < (numVertices + 1); i++) {
		for (int j = 0; j < (numVertices + 1); j++) {
			graph[i][j] = 0;
		}
	}

	//create the adjacency matrix from a matrix
	int i = 0, j = 0;
	for (int p = 1; p <= numVertices; p++) {

		retrieve_coordinate(&i, &j, p, cloudMap);

		if (j + 1 < cols) { //casella destra
			graph[p][cols * i + (j + 1) + 1] = cloudMap[i][j];
		}
		if (j - 1 >= 0) { //casella sinistra
			graph[p][cols * i + (j - 1) + 1] = cloudMap[i][j];
		}
		if (i + 1 < rows) { //casella sotto
			graph[p][cols * (i + 1) + j + 1] = cloudMap[i][j];
		}
		if (i - 1 >= 0) { //casella sopra
			graph[p][cols * (i - 1) + j + 1] = cloudMap[i][j];
		}

		//diagonals
		if (i - 1 >= 0 && j - 1 >= 0) { //angolo alto sinistra
			graph[p][cols * (i - 1) + j - 1 + 1] = cloudMap[i][j];
		}
		if (i - 1 >= 0 && j + 1 < cols) { //angolo alto destra
			graph[p][cols * (i - 1) + j + 1 + 1] = cloudMap[i][j];
		}
		if (i + 1 < rows && j - 1 >= 0) { //angolo basso sinistra
			graph[p][cols * (i + 1) + j - 1 + 1] = cloudMap[i][j];
		}
		if (i + 1 < rows && j + 1 < cols) { //angolo basso destra
			graph[p][cols * (i + 1) + j + 1 + 1] = cloudMap[i][j];
		}
	}

	matrix = (int**)malloc((numVertices + 1) * sizeof(int*)); //new int*[numVertices + 1];
	for (int i = 0; i < (numVertices + 1); i++)
		matrix[i] = (int*)malloc( (numVertices + 1) * sizeof(int)); //new int*[numVertices + 1];

	//Delete the first row and first column (that are only 0 values) -> create the adjacency matrix
	for (int i = 1; i < (numVertices + 1); i++) {
		for (int j = 1; j < (numVertices + 1); j++) {
			matrix[i - 1][j - 1] = graph[i][j];
		}
	}

	return matrix;
}

//function that retrieve value(cost) from a position 
//@Param: position of vertex in graph
//@Return: the value at that position
int dijkstra::getValueMatrix(int pos) {
	int cnt = 0;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			//printf("%d ", cloudPositionMap[i][j]);
			if (cnt == pos) {
				return cloudPositionMap[i][j];
			}
			cnt++;
		}
		//printf("\n");
	}
	return 0;
}

int dijkstra::findPosition(Point** cloud, Point pp, int row, int col) {
	int pos = 0;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (cloud[i][j].getLatitude() == pp.getLatitude() && cloud[i][j].getLongitude() == pp.getLongitude()) { return pos; }
			pos++;
		}
	}

	std::cout << "\n!!!!!    Punto non sulla mappa    !!!!!\n" << endl;
 	return -1;
}

/**MAIN function of this class
@param1: weather map
@param2: number rows map
@param3: number cols map
@param4: source position
@param5: destination position
*/
Point* dijkstra::calculatePath(Point** cloud, int h, int w, Point source, Point destination) {

	rows = h; //rows of map - height
	cols = w; //cols of map - width
	int src = findPosition(cloud, source, h, w);
	int dest = findPosition(cloud, destination, h, w);
	if (src == -1 || dest == -1) { std::cout << "\nPunto INIZIO o FINE non sulla mappa\n" << endl; return nullptr; }

	numVertices = h * w; //number vertices of graph: each position on map is a vertex
	path = (int*)malloc((numVertices + 1) * sizeof(int)); //new int[numVertices + 1];

	int** cloudMap = new int*[rows];
	for (int i = 0; i < rows; i++) {
		cloudMap[i] = new int[cols];
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
				cloudMap[i][j] = cloud[i][j].getCloudPercentage();
		}
	}

	printf("number of vertices in the matrix: %d\n", numVertices);

	//remove zero point on map
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (cloudMap[i][j] == 0) {
				cloudMap[i][j] = 1;
			}
		}
	}

	matrix = createGraph(cloudMap);

	//Print debug only cloud map
	if (DISPLAY_CLOUD) {
		printf("\nStampa mappa nuvole:\n");
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				printf("%d ", cloudPositionMap[i][j]);
			}
			printf("\n");
		}
	}

	//Print debug only adjacency matrix
	if (DISPLAY_ADJ) {
		printf("\nStampa matrice adiacenze:\n");
		for (int i = 0; i < numVertices; i++) {
			for (int j = 0; j < numVertices; j++) {
				printf("%d ", matrix[i][j]);
			}
			printf("\n");
		}
	}

	//calculate all possible combination source - destination -> explore all possible path
	if (DEBUG_DIJKSTRA) {
		for (int j = 0; j < numVertices; j++) {
			for (int i = 0; i < numVertices; i++) {
				printf("Origine: %d, destinazione: %d \n", i, j);
				dijkstraAlgorithm(matrix, i, j);
				printf("\n\n");
			}
		}
	}

	dijkstraAlgorithm(matrix, src, dest);
	path = getShortestPath();

	Point* route = pathToCoordinate(cloud, path);

	if (DISPLAY_PATH) {
		printf("\nPercorso: \n");
		displayPathOnConsole(cloudMap);
	}


	return route;
}

Point* dijkstra::pathToCoordinate(Point** cloud, int* path) {
	Point* route = new Point[numVertices];
	int cnt = 0;
	
	for (int k = 0; k < lengthPath; k++) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (cnt==path[k]) {
					route[k] = cloud[i][j];
				}
				cnt++;
			}
		}
		cnt = 0;
	}
	
	return route;
}

//DEBUG ONLY -> Visual debug
//Display the path on video -> DEBUG only
void dijkstra::displayPathOnConsole(int** matrix) {

	printf("Path: ");
	printf("%d ", sourceVertex);
	for (int i = 0; i < lengthPath; i++) {
		printf("%d ", path[i]);
	}
	printf(" - costo totale: %d \n", costPath);

	//printf("\nDisplay path ordered: \n\n");
	int n1 = 0, n2 = 0, temp = 0;

	//order array path - bubble sort
	for (n1 = 0; n1 < lengthPath; n1++) {
		for (n2 = 0; n2 < lengthPath - n1 - 1; n2++) {
			if (path[n2] > path[n2 + 1]) {  //Scambio valori
				temp = path[n2];
				path[n2] = path[n2 + 1];
				path[n2 + 1] = temp;

			}
		}
	}
	
	printf("\n");
	int cnt = 0, k = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (cnt == sourceVertex) {
				printf("%d ", matrix[i][j]);
			} else if (path[k] == cnt ) {
				printf("%d ", matrix[i][j]);
				k++;
			}
			else {
				printf(". ");
			}
			cnt++;
		}
		printf("\n");
	}
}

//return the matrix adjacency
int** dijkstra::getMatrix() {
	return matrix;
}

//return the lenght path
int dijkstra::getLenghtPath() {
	return lengthPath;
}