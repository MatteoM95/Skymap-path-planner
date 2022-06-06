#include "gribURLManage.h"
#include <iostream>
#include <Windows.h>
#include <string>

#pragma comment(lib, "urlmon.lib") // for reference: https://stackoverflow.com/questions/32667035/c-download-file-in-directory

gribURLManage::gribURLManage(){}

std::string gribURLManage::gribURLcreation(std::string hour,
									  std::string date,
									  int topLatitude,
									  int bottomLatitude,
									  int leftLongitude,
									  int rightLongitude
									  //,int altitude
									  ) {


	if ( stoi(hour) > 120) { char hour [] = "119"; }

	std::string urlRoot = "https://nomads.ncep.noaa.gov/"; //root url domain
	std::string formatType = "cgi-bin/filter_gfs_0p25.pl?"; // grib 0.25 degree filter hourly
	std::string fileName = "file=gfs.t00z.pgrb2.0p25.f" + std::string(hour); // filename downloaded
	std::string altitudeParam = "&all_lev=on"; //+ std::to_string(altitude) + "_mb=on";
	std::string infoParam = "&var_TCDC=on"; // parameter choose TCDC = cloud cover percentage (view documentation for other parameter)
	std::string areaParam = "&subregion=&leftlon=" + std::to_string(leftLongitude) + "&rightlon=" + std::to_string(rightLongitude) + "&toplat=" + std::to_string(topLatitude) + "&bottomlat=" + std::to_string(bottomLatitude); // region bounds
	std::string dateParam = "&dir=%2Fgfs." + date + "%2F00%2Fatmos"; // date paramenter

	std::string URL = urlRoot + formatType + fileName + altitudeParam + infoParam + areaParam + dateParam;

	//std::cout << "URL :" << URL << endl;

	return URL;
}

void gribURLManage::downloadGRIB(std::string urlDownload, std::string pathWhereToSaveGrib) {

	std::wstring dwnld_URL = std::wstring(urlDownload.begin(), urlDownload.end());
	std::wstring filePath = std::wstring(pathWhereToSaveGrib.begin(), pathWhereToSaveGrib.end());

    // if error: cannot convert const wchar_t* to LPCSTR:  in VS2019 set Conformance Mode to Default in the project Properties -> C/C++ -> Language.
    // https://stackoverflow.com/questions/62579746/initializing-cannot-convert-from-const-wchar-t35-to-lpwstr
	URLDownloadToFile(NULL, dwnld_URL.c_str(), filePath.c_str(), 0, NULL);

	return;
}
