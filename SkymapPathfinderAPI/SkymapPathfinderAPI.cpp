// Get weather from weather API, polling each square cloud percentage

//https://stackoverflow.com/questions/50013204/jsoncpp-to-parse-from-url
//https://github.com/gabrieledarrigo/c-weather
//https://gist.github.com/connormanning/41efa6075515019e499c solo per presa visione

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "WeatherAPI.h"

int rows = 10;
int cols = 10;
int **matrix;
WeatherAPI weather = WeatherAPI();

int latitude = 45, longitude = 7;

int main() {

	printf("Cloud Matrix:	\t Rows: %d \t Cols: %d\n", rows, cols);
	matrix = (int**) malloc(rows * sizeof(int*));
	for (int i = 0; i < rows; i++) {
		matrix[i] = (int*) malloc(cols * sizeof( int ));
		matrix[i][cols - 1] = '\0';
	}

	//NOTE 0.2 degree square grid
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			weather.createUrl(latitude + j*0.2, longitude + i*0.2);
			int c = weather.getCloudCondition();
			matrix[i][j] = c;
			printf("%d ", c);
		}
		printf("\n");
	}

	printf("Completed");
	free(matrix);

	return 0;
}
