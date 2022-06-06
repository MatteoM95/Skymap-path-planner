#include "degribClass.h"
#include "Point.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream> 

using namespace std;

degribClass::degribClass(){}

//Convert download file grib2 to CSV file
void degribClass::convertToCSV(std::string gribName, std::string path, int param, std::string csvFilename) {

	std::string command;
	std::string gribName_t(gribName);
	std::string path_t(path);
	std::string csv_t(csvFilename);
	std::string param_t;
	param_t.push_back((char)(param + '0'));

	//command = gribName_t + " -C -msg " + param_t + " -Csv";
	command = path_t + "\\degrib.exe " + path_t + "\\" + gribName_t + " -C -msg " + param_t + " -Csv -out " + csv_t;
	std::cout << "\n" + command << endl;

	system(command.c_str());
}

//Read the CSV file with cloud percentege and store in grid (matrix) the cloud map
Point** degribClass::readCSV(std::string path, std::string fileName, int* row, int* col) {

	matrix = new Point*[MAX_DIM_MATRIX];
	for (int i = 0; i < MAX_DIM_MATRIX; i++) {
		matrix[i] = new Point[MAX_DIM_MATRIX];
	}

	std::string path_;
	//strcpy(path_, path);
	//strcat(path_, "\\");
	//strcat(path_, fileName);

	path_ = path + "\\" + fileName;
	std::cout << "Path CSV:    " << path_ << endl;

	int x_, y_, maxRow=0, maxCol=0;
	float lat, lon;
	int cloudPer;
	
	ifstream file(fileName);
	//file.open(fileName, ios::in);
	if (!file) { cout << "Error in opening file!!!" << endl; return 0; }

	std::string str;
	std::getline(file, str);

	//read and print file content
	while (std::getline(file, str, ',')) {
		stringstream geek1(str);
		geek1 >> x_;
		if (x_ > maxCol) { maxCol = x_; }
		//cout << "X: " << x_ << "  -  ";

		std::getline(file, str, ',');
		stringstream geek2(str);
		geek2 >> y_;
		if (y_ > maxRow) { maxRow = y_; }
		//cout << "Y: " << y_ << "  -  ";

		std::getline(file, str, ',');
		stringstream geek3(str);
		geek3 >> lat;
		//cout << "Lat: " << lat << "  -  ";

		std::getline(file, str, ',');
		stringstream geek4(str);
		geek4 >> lon;
		//cout << "Lon: " << lon << "  -  ";

		std::getline(file, str);
		stringstream geek5(str);
		geek5 >> cloudPer;
		//cout << "Cloud: " << cloudPer << endl;

		Point ppp(cloudPer, lat, lon, x_, y_);
		//ppp.stampa();

		matrix[y_ - 1][x_ - 1] = ppp;
	}
	file.close();

	//inversione della matrice per creare piano XY
	int tmp=maxRow-1;
	Point** tmpMatrix = new Point * [MAX_DIM_MATRIX];
	for (int i = 0; i < MAX_DIM_MATRIX; i++) {
		tmpMatrix[i] = new Point[MAX_DIM_MATRIX];
	}

	for (int i = 0; i < maxRow; i++) {
		for (int j = 0; j < maxCol; j++) {
			tmpMatrix[tmp][j] = matrix[i][j];
			//std::cout << tmpMatrix[tmp][j].getCloudPercentage() << " ";
		}
		tmp--;
		//std::cout << endl;
	}

	*row = maxRow;
	*col = maxCol;


	return tmpMatrix;
}
