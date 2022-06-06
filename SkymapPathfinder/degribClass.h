#include <string>
#include <iostream>
#include "Point.h"

#define MAX_DIM_MATRIX 1000

class degribClass {

	public:
		degribClass();
		void convertToCSV(std::string gribName, std::string path, int param, std::string csvFilename);
		Point** readCSV(std::string path, std::string fileName, int* row, int* col);

		Point** matrix;
	private:



};

