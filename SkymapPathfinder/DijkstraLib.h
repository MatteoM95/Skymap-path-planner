#ifndef DIJKSTRALIB
#define DIJKSTRALIB

#include "Point.h"

//Class Dijkstra
class dijkstra {
public:
	int getMinimumVertex(bool mst[], int key[]);
	int* getShortestPath();
	void printPath(int parent[], int j);
	void printDijkstra(int sourceVertex, int key[], int* parent);
	void dijkstraAlgorithm(int** matrix, int sourceVer, int dest);
	void createShortestPath(int* parent, int j, int i);
	int** createGraph(int** cloudMap);
	void retrieve_coordinate(int* i, int* j, int p, int** m);
	Point* calculatePath(Point** cloudMap, int h, int w, Point src, Point dest);
	Point* pathToCoordinate(Point** cloud, int* path);
	int findPosition(Point** cloud, Point pp, int row, int col);
	void displayPathOnConsole(int** matrix);
	int** getMatrix();
	int getValueMatrix(int pos);
	int getLenghtPath();

	//GRAPH
	int rows = 0;   //rows adjacency matrix
	int sourceVertex = 0;	//vertex of start flight
	int destinationVertex = 0;	//vertex of ending flight
	int cols = 0;       //cols adjacency matrix
	int numVertices = 0;//number vertices (nodes) in adjacency matrix
	int* path;          //the shortest path
	int lengthPath = 0; //the length of path
	int** matrix;		//adjacency matrix of graph
	int** cloudPositionMap; //cloud matrix with percentage cloud
	int costPath = 0;

};

#endif