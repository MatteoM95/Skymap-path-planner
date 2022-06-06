#pragma once
#include <string>
#include <iostream>

using namespace std;

class gribURLManage{

	public:
		gribURLManage();
		string gribURLcreation(std::string hour, std::string date, int topLatitude, int bottomLatitude, int leftLongitude, int rightLongitude); //, int altitude);
		void downloadGRIB(std::string urlDownload, std::string pathWhereToSaveGrib);

	private:

};

