/*
Class aim: Download grib file from given URL
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace SkymapPathfinderGUI {
    public class gribURLManage {

            //constructor
            public gribURLManage(){}
            
            //creation of URL for download
            public string gribURLcreation( string hour,
                                           int date,
                                           float topLatitude,
                                           float bottomLatitude,
                                           float leftLongitude,
                                           float rightLongitude
                                           ) {

                if (int.Parse(hour) > 120) { hour = "119"; }

                string urlRoot = "https://nomads.ncep.noaa.gov/";
                string formatType = "cgi-bin/filter_gfs_0p25.pl?"; // grib 0.25 degree filter
                string fileName = "file=gfs.t00z.pgrb2.0p25.f" + hour;
                string altitudeParam = "&all_lev=on"; //+ std::to_string(altitude) + "_mb=on";
                string infoParam = "&var_TCDC=on";
                string areaParam = "&subregion=&leftlon=" + leftLongitude + "&rightlon=" + rightLongitude + "&toplat=" + topLatitude + "&bottomlat=" + bottomLatitude;
                string dateParam = "&dir=%2Fgfs." + date + "%2F00%2Fatmos";
                string URL = urlRoot + formatType + fileName + altitudeParam + infoParam + areaParam + dateParam;

                //Console.WriteLine("Link grib: {0}", URL);

                return URL;
            }
            
            //Download file grib to computer
            public void downloadGRIB(string urlDownload, string pathWhereToSaveGrib) {

                WebClient client = new WebClient();
                try {
                    client.DownloadFile(urlDownload, pathWhereToSaveGrib);
                } catch (Exception e)        {
                    Console.WriteLine("NON connesso");
                }

                return;
            }
}
}
