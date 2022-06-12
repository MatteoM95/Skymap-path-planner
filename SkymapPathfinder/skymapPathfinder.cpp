//VS2019: in case of this warning, disable _CRT_SECURE_NO_WARNING in project properties -> C/C++ -> preprocessor -> add _CRT_SECURE_NO_WARNING to definition to preprocessor
//https://stackoverflow.com/questions/16883037/remove-secure-warnings-crt-secure-no-warnings-from-projects-by-default-in-vis

// command line: SkymapPathfinder.exe 030 48 40 10 20 25 C:\Icarus 46 11 42 19.5

#include <windows.h>
#include <iostream>
#include <fstream>
#include <ctime>

#include "degribClass.h"
#include "gribURLManage.h"
#include "Point.h"
#include "DijkstraLib.h"

using namespace std;

std::string hour = "030"; //forecast hour from now (max 120) - format must be 3 digit
char date [100]; //year month day (forecast date)

// rectangle where to map the cloud percentage
int topLatitude = 48; //degree
int bottomLatitude = 40; //degree
int leftLongitude = 10; //degree
int rightLongitude = 20; //degree

int parameterToExtract = 25; //parameter of height in millibar (max 22) see the documentation

std::string fileName = "cloud.grb2";
std::string csvFile = "cloud.csv";
std::string savePath = "C:\\Icarus"; // location where program degrib.exe is stored

 //Default points start and finish
Point start(0, 47, 11, 0, 0);
Point finish(0, 41, 19, 0, 0);

Point** cloudMatrix;
Point* route;
int rows, cols;

int main(int argc, char *argv[]) {

	if (argc > 1) {
		hour = argv[1];
		topLatitude	= atoi(argv[2]);
		bottomLatitude = atoi(argv[3]);
		leftLongitude = atoi(argv[4]);
		rightLongitude = atoi(argv[5]);
		parameterToExtract = atoi(argv[6]);
		savePath = argv[7];
		float startLatPoint = atof(argv[8]);
		float startLonPoint = atof(argv[9]);
		float finishLatPoint = atof(argv[10]);
		float finishLonPoint = atof(argv[11]);
		start.setLatitude(startLatPoint);
		start.setLongitude(startLonPoint);
		finish.setLatitude(finishLatPoint);
		finish.setLongitude(finishLonPoint);
	}

	const int MAXLEN = 80;
	char date[MAXLEN];
	time_t t = time(0);
	strftime(date, MAXLEN, "%Y%m%d", localtime(&t));
	std::cout << date << '\n';


	std::string path; // location where to download grib file

	gribURLManage gribManage;
	degribClass degrib;
	dijkstra dij;

	std::string URL = gribManage.gribURLcreation(hour, date, topLatitude, bottomLatitude, leftLongitude, rightLongitude);
	std::cout << "Download URL grib:\n" + URL << endl;

	//strcpy(path, savePath);
	//strcat(path, "\\");
	//strcat(path, fileName);
	path = savePath + "\\" + fileName;

	gribManage.downloadGRIB(URL, path);

	Sleep(100);

	degrib.convertToCSV(fileName, savePath, parameterToExtract, csvFile);

	Sleep(100);

	cloudMatrix = degrib.readCSV(savePath, csvFile, &rows, &cols);

	/*
	//stampa matrice
	std::cout << "\n\nMATRICE nuvole 1: righe: " << rows << " - colonne: " << cols << "\n\n";
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			std::cout << cloudMatrix[i][j].getCloudPercentage() << " ";
		}
		std::cout << endl;
	}
	*/

	route = dij.calculatePath(cloudMatrix, rows, cols, start, finish);

	std::cout << "\n\n Percorso con " << dij.getLenghtPath() << " punti:\n\n" << endl;
	for (int i = 0; i < dij.getLenghtPath(); i++) {
		route[i].stampa();
	}

	ofstream outputFile;
	outputFile.open("trackPosition.csv");
	for (int i = 0; i < dij.getLenghtPath(); i++) {
		outputFile << route[i].getLatitude() << ", " << route[i].getLongitude() << endl;
	}
	outputFile.close();

	std::cout << "Completed" << endl;
	getchar();

	return 0;
}
