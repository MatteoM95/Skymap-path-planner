/*
 * Dijkstra algorithm class: calculate shortest path  between two point
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace SkymapPathfinderGUI {
    public class Dijkstra {
        
        //DEBUG FLAG
        static bool DEBUG_DIJKSTRA = false;		    //use ONLY!! on small matrix -> view all possible path
        static bool DISPLAY = true;				    //display on console the path
        static bool DISPLAY_ADJ = false;			//display adj matrix
        static bool DISPLAY_CLOUD = true;			//display cloud map
        static bool DISPLAY_PATH = true;            //draw on console the path over the matrix

        static int INT_MAX = 100000;

        //GRAPH VARIABLE
        int rows = 0;                               //rows adjacency matrix
        int sourceVertex = 0;                       //vertex of start flight
        int destinationVertex = 0;                  //vertex of ending flight
        int cols = 0;                               //cols adjacency matrix
        int numVertices = 0;                        //number vertices (nodes) in adjacency matrix
        int[] path;                                 //the shortest path
        int lengthPath = 0;                         //the length of path
        float[,] matrix;                            //adjacency matrix of graph
        int[,] cloudPositionMap;                    //cloud matrix with percentage cloud
        float costPath = 0;

        
        public Dijkstra() { } //Constructor

            /**
        Create the shortest path from the source - NOTE: the path is reversed
        @Param1: parents vertices visited, best path
        @Param2: destination position
        @Param3: indexing used for storing values in path array
        */
        public void createShortestPath(int[] parent, int j, int i) {
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
         public int[] getShortestPath() {
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
                int[] realPath = new int [length]; //(int*)malloc((length + 1) * sizeof(int)); //new int[length + 1];
                int j = 0;
                for (int i = length; i > 0; i--) {
                    realPath[j] = path[i];
                    j++;
                }

                return realPath;
            }
        

            /**
        Get the vertex with minimum distance which is not included in SPT
        @Param1: minimum spanning tree array
        @Param2: distance array
        @Return: the minimum vertex
        */
          public int getMinimumVertex(bool [] mst, float[] key)  {
                float minKey = INT_MAX;
                int vertex = -1;
                for (int i = 0; i < numVertices; i++)
                {
                    if (mst[i] == false && minKey >= key[i])
                    {
                        minKey = key[i];
                        vertex = i;
                        //printf("minkey %d > key %d , mst[%d] bool: %d\n",minKey, key[i], i, mst[i]); //ELIMIANNNNNNNNNNNNNN
                    }
                }
                return vertex;
            }

            /**
        Dijkstra Algorithm
        @param1: adjacency matrix
        @param2: source position
        @param3: destination position
        */
           public void dijkstraAlgorithm(float[,] matrix, int sourceVer, int dest) {

            float[] distance = new float[numVertices + 2]; //new int[numVertices];     //array of distance from every node
            int[] parent = new int[numVertices + 2]; //new int[numVertices];      //array with all previous node visit
            bool[] spt = new bool[numVertices + 2]; //new bool[numVertices];      //array of spanning tree visit

            sourceVertex = sourceVer;
            destinationVertex = dest;

                //Initialize all the distance to infinity
                for (int i = 0; i < numVertices + 1; i++) {
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
                        if (matrix[vertex_U, vertex_V] > 0 && spt[vertex_V] == false && matrix[vertex_U, vertex_V] != INT_MAX) {

                            //check if distance needs an update or not means check total weight from source to vertex_V is less than
                            //the current distance value, if yes then update the distance

                            float newKey = matrix[vertex_U, vertex_V] + distance[vertex_U];
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
        Retrieve the row and column (i, j) of certain position p on map -> transformation of map in SymbolTable matrix
        @param1 and @param2: by reference is passed the position on map
        @param3 is the position to be searched
        @param4 is the weather map matrix
        */
            public void retrieve_coordinate(ref int i, ref int j, int p, int[,] m) { 
                
                /*
                 * Matrix CloudAdjacency [3,4]    ->    Matrix SymbolTable
                 * 
                 * 2 5 7                                  0 1 2
                 * 1 9 8                                  3 4 5
                 * 5 4 3                                  6 7 8
                 * 2 3 6                                  9 10 11  
                 * 
                 * get(2) -> 7
                 * get(4) -> 9
                 */         

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
                for (int s = 0; s < rows; s++)
                {
                    for (int g = 0; g < cols; g++)
                    {
                        cnt++;
                        if (cnt == p)
                        {
                            i = s;
                            j = g;
                            return;
                        }
                    }
                }
            }

            /**
        Create the Adjacency matrix from the map matrix
        @Param1: cloud map
        @Return: the weighted adjacency matrix (the graph)
        */
            public float[,] createGraph(int[,] cloudMap) {

            float[,] graph = new float[numVertices+1, numVertices+1]; //Matrix of adjacency

            cloudPositionMap = new int[cols, rows];
            cloudPositionMap = cloudMap;

            //reset to zero the graph
            for (int t = 0; t < (numVertices + 1); t++) {
                for (int g = 0; g < (numVertices + 1); g++) {
                        graph[t, g] = 0;
                }
            }

            //create the adjacency matrix from a matrix, calculate the weight of links, considering half weight per cell
            int i = 0, j = 0;
            for (int p = 1; p <= numVertices; p++) {

                retrieve_coordinate(ref i, ref j, p, cloudMap);

                    if (j + 1 < cols) { //casella destra
                         graph[p, cols * i + (j + 1) + 1] = (float)((cloudMap[i, j] + cloudMap[i, j + 1]) /2.0);
                    }
                    if (j - 1 >= 0) { //casella sinistra
                        graph[p, cols * i + (j - 1) + 1] = (float)((cloudMap[i, j] + cloudMap[i, j - 1]) / 2.0);
                    }
                    if (i + 1 < rows) { //casella sotto
                        graph[p, cols * (i + 1) + j + 1] = (float)((cloudMap[i, j] + cloudMap[i + 1, j]) / 2.0);
                    }
                    if (i - 1 >= 0) { //casella sopra0
                        graph[p, cols * (i - 1) + j + 1] = (float)((cloudMap[i, j] + cloudMap[i - 1, j]) / 2.0);
                    }

                    //diagonali
                    if (i - 1 >= 0 && j - 1 >= 0) { //angolo alto sinistra
                        graph[p, cols * (i - 1) + j - 1 + 1] = (float)((cloudMap[i, j] + cloudMap[i - 1, j - 1]) / 2.0);
                    }
                    if (i - 1 >= 0 && j + 1 < cols) { //angolo alto destra
                        graph[p, cols * (i - 1) + j + 1 + 1] = (float)((cloudMap[i, j] + cloudMap[i - 1, j + 1]) / 2.0);
                    }
                    if (i + 1 < rows && j - 1 >= 0) { //angolo basso sinistra
                        graph[p, cols * (i + 1) + j - 1 + 1] = (float)((cloudMap[i, j] + cloudMap[i + 1, j - 1]) / 2.0);
                    }
                    if (i + 1 < rows && j + 1 < cols) { //angolo basso destra
                        graph[p, cols * (i + 1) + j + 1 + 1] = (float)((cloudMap[i, j] + cloudMap[i + 1, j + 1]) / 2.0);
                    }
                }

            matrix = new float[numVertices + 1, numVertices + 1]; //new int*[numVertices + 1];

                //Delete the first row and first column (that are only 0 values) -> create the adjacency matrix
                for (i = 1; i < (numVertices + 1); i++) {
                    for (j = 1; j < (numVertices + 1); j++) {
                        matrix[i - 1, j - 1] = graph[i, j];
                        
                    }
                }
                
                return matrix;
            }

            /**
        Retrieve value(cost) from a certain position 
        @Param: position of vertex in graph
        @Return: the value at that position
        */
            public  int getValueMatrix(int pos) {
                int cnt = 0;

                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        //printf("%d ", cloudPositionMap[i][j]);
                        if (cnt == pos) {
                            return cloudPositionMap[i, j];
                        }
                        cnt++;
                    }
                    //printf("\n");
                }
                return 0;
            }

          public int findPosition(Point[,] cloud, Point pp, int row, int col) {
                int pos = 0;

                for (int i = 0; i < row; i++) {
                    for (int j = 0; j < col; j++) {
                        if (cloud[i, j].getLatitude() == pp.getLatitude() && cloud[i, j].getLongitude() == pp.getLongitude()) { return pos; }
                        pos++;
                    }
                }

                Console.WriteLine("\n!!!!!    Point out of border's map    !!!!!\n");
                return -1;
            }

            /**MAIN function of this class
        @param1: weather map
        @param2: number rows map
        @param3: number cols map
        @param4: source position
        @param5: destination position
        @Return: the route with all coordinates
        */
        public Point[] calculatePath(Point[,] cloud, int h, int w, Point source, Point destination) {

                rows = h; //rows of map - height
                cols = w; //cols of map - width
                int src = findPosition(cloud, source, h, w);
                int dest = findPosition(cloud, destination, h, w);
                if (src == -1 || dest == -1) { Console.WriteLine("\nPunto INIZIO o FINE non sulla mappa\n"); return null; }

                numVertices = h * w; //number vertices of graph: each position on map is a vertex
                path = new int[numVertices + 2]; //new int[numVertices + 1];

                int[,] cloudMap = new int[rows + 1, cols + 1];
            /*
                for (int i = 0; i < rows; i++)
                {
                    cloudMap[i] = new int[cols];
                }*/
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                            cloudMap[i, j] = (int)cloud[i, j].getCloudPercentage();
                        }
                }

                Console.Write("Number of vertices in the matrix: {0}\n", numVertices);

                //remove zero point on map
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        if (cloudMap[i, j] == 0) {
                            cloudMap[i, j] = 1;
                        }
                    }
                }

                matrix = createGraph(cloudMap);

                //Print debug only cloud map
                if (DISPLAY_CLOUD) {
                    Console.Write("\nStampa mappa nuvole:\n");
                    for (int i = 0; i < rows; i++) {
                        for (int j = 0; j < cols; j++) { 
                        Console.Write("{0} ", cloudPositionMap[i, j]);
                        }
                    Console.Write("\n");
                    }
                }

                //Print debug only adjacency matrix
                if (DISPLAY_ADJ) {
                    Console.Write("\nStampa matrice adiacenze:\n");
                    for (int i = 0; i < numVertices; i++) {
                        for (int j = 0; j < numVertices; j++) {
                        Console.Write("{0} ", matrix[i, j]);
                        }
                    Console.Write("\n");
                    }
                }

                //calculate all possible combination source - destination -> explore all possible path
                if (DEBUG_DIJKSTRA) {
                    for (int j = 0; j < numVertices; j++) {
                        for (int i = 0; i < numVertices; i++) {
                            Console.Write("Source: {0}, destination: {1} \n", i, j);
                            dijkstraAlgorithm(matrix, i, j);
                            Console.Write("\n\n");
                        }
                    }
                }

                dijkstraAlgorithm(matrix, src, dest);
                path = getShortestPath();

                Point[] route = pathToCoordinate(cloud, path);

                if (DISPLAY_PATH) {
                    Console.Write("\nPath: \n");
                    displayPathOnConsole(cloudMap);
                }


                return route;
            }

            /**Convert index on map to real coordinates
        @param1: weather map
        @param2: all route vertices index
        @Return: the route with all coordinate
        */
        public Point[] pathToCoordinate(Point[,] cloud, int[] path) {
                Point[] route = new Point[numVertices];
                int cnt = 0;

                for (int k = 0; k < lengthPath; k++) {
                    for (int i = 0; i < rows; i++) {
                        for (int j = 0; j < cols; j++) { 
                            if (cnt == path[k]) {
                                route[k] = cloud[i, j];
                            }
                            cnt++;
                        }
                    }
                    cnt = 0;
                }
                return route;
            }
    
            /**
        Recursive function that print on console the path
        @Param: parents array, vertex previous visited
        @Param: source vertex / previous vertex
        */
        public void printPath(int[] parent, int j) {

            // Base Case : If j is source
            if (parent[j] == -1) {
                return;
            }

            printPath(parent, parent[j]);
            Console.Write("%d ", j);
        }

            /**
        Print all possible path from that source point
        @Param1: source point
        @Param2: distance array
        @Param3: parents array, vertices previous visited
        */
        public void printDijkstra(int sourceVertex, float[] key, int[] parent) {

            Console.WriteLine("Result of all path\n");
            for (int i = 0; i < numVertices; i++)  {
                Console.Write("Source %d -> %d, distance: %d -- path %d ", sourceVertex, i, key[i], sourceVertex);
                printPath(parent, i);
                Console.Write("\n");
            }
        }

        //DEBUG ONLY -> Console debug
        //Display the path on video -> DEBUG only
        public void displayPathOnConsole(int[,] matrix) {

                Console.Write("Path: " + sourceVertex);
                for (int i = 0; i < lengthPath; i++) {
                    Console.Write(" {0}", path[i]);
                }
                Console.Write(" - Total cost: " + costPath + ", with " + lengthPath + " point; origin vertex " + sourceVertex + "\n");

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

            int cnt = 0, k = 0;
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        if (cnt == sourceVertex) {
                            Console.Write("{0} ", matrix[i, j]);
                        } else if (path[k] == cnt && k<(path.Length-1) ) {
                            Console.Write("{0} ", matrix[i, j]);
                        k++;
                        } else {
                            Console.Write(". ");
                        }
                        cnt++;
                    }
                    Console.Write("\n");
                }
            }

            //return the matrix adjacency
            public float[,] getMatrix() {
                return matrix;
            }

            public int getLenghtPath() { 
                return this.lengthPath;
            }
    }
}


// END CLASS