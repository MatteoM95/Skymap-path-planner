# Skymap-path-planner
<!--- [Alt Text](https://github.com/MatteoM95/Skymap-path-planner/blob/main/Media/path.gif) --->

### Overview
The following program calculate the best path between two given point on a weighted matrix. The weighted matrix represents the percentage of clouds in the sky in certain moment. The UAV uses photovoltaic power to fly and should avoid clouds in upper level in the sky.
The weather is provided by the American NOAA service in grib2 file format (file.grb2). 

Xplane has its own [weather plugin](https://x-plane.joanpc.com/plugins/xpgfs-noaa-weather), which uses the same data, to install it you need to get the zip file and manually copy the files to the folder. Here is a [tutorial](https://www.youtube.com/watch?v=8Nv_fM4RtaQ) how to install it 

The grib files are binary files and it is necessary to "unpack" them (degrib, like unzipping file) and save the interested data in a .csv or ASCII file. To visualize grib files use the [XyGrib GUI](https://opengribs.org/en/).

The NOAA site freely provides data with maximum resolution 0.25 degrees which correspond to squares of 30 Km per side. Higher resolution is provided by French system via [AROME](https://donneespubliques.meteofrance.fr/?fond=produit&id_produit=131&id_rubrique=51) 0.025 degree  
Other resolutions are available [here](https://opengribs.org/en/gribs)

### Download GRIB file
The grib files are generated by NOAA and downloaded through the [website](https://nomads.ncep.noaa.gov/)
The precision selected is Gfs 0.25 degree [filter](https://nomads.ncep.noaa.gov/cgi-bin/filter_gfs_0p25.pl)
Then select the most recent date of the last update and hours. The forecast bulletin are only available for the last ten days and then updated every 6 hours: from midnight (00) until 18 PM in the evening (18). The actual forecast is only avalaible for the next 120 hours from the meteorological sampling date.
**WARNING**: do not select the .anl file, but only those ending with f000 - f0120
Now select the parameter interested, such as barometric pressure and altitude. Here is available a parameter [table](https://www.nco.ncep.noaa.gov/pmb/docs/on388/table2.html)
Select the region by ticking the box and then download. There is also the possibility to inspect the corresponding URL, checking the box next to it.

The file will be downloaded without extension, a .grb2 extension should be added to the end of filename. Now the file is displayed as grb2 and can be visualize in Xygrib.

### Degrib the file
Here there is the official NOAA page about [degrib](https://vlab.noaa.gov/web/mdl/degrib) and [tutorial](https://vlab.noaa.gov/web/mdl/degrib-tutorial) how to use it 

The degrib program is used to decompresses and convert it in ASCII.
Download and [install](https://vlab.noaa.gov/web/mdl/degrib-install) Degrib program: 
Two executables will be created (in the installation folder) from the installation: 
 - degrib.exe
 - tkdegrib.exe

The first program is runned from the command line, the second is the same program with the user interface. 
**WARNING**: The degrib.exe file must be in the same folder of compiled code of the Skymap path planner.
##### Convert binary file to ASCII Comma Separated file:
You can use degrib to convert a given message to a .csv file, which each line contains x, y, lat, lon, value for each cell of the map. 
 - In the GUI tkdegrib:
Click on the "GIS" tab, and browse for your file. Double click on it in the top half, and it should fill out the inventory part in the bottom half. Select the message that you want in the bottom half, and choose the output name (or press "Recommend" to have tkdegrib "recommend" one. Next choose "CSV" for "File Type", and press the "Generate .csv file" button at the bottom. The TkDegrib manual is available [here](https://vlab.noaa.gov/web/mdl/tk-degrib-man-page)
 - For the Degrib.exe use the following command lines:
```
/degrib /bin /degrib GRIB_filename.grb2 -C -msg 1 -Csv
/degrib /bin /degrib GRIB_filename.grb2 -C -msg all -nMet -Csv
```
where "GRIB_filename" is replaced with your GRIB filename.

The -Csv option tells it to create the .csv file. See the [Degrib manual](https://vlab.noaa.gov/web/mdl/degrib-man-page), in particular the "CONVERT OPTIONS" section.

### Skymap path planner code
There are various code:
 - A program executable in CPP with command line parameters [here](https://github.com/MatteoM95/Skymap-path-planner/tree/main/SkymapPathfinder)
 - The same program with a GUI in C # with dijkstra [here](https://github.com/MatteoM95/Skymap-path-planner/tree/main/SkymapPathfinderGUI)
 - A program with the openweather.org API has also been created test but never used  

In the CPP SkymapPathfinder.exe command line the parameters passed are in order:

0. Program name and path folder
1. Hours from now forecast (max 120) and must be 3 digit ie. 030
2. North latitude area to explore
3. South latitude area to explore
4. West longitude area to explore
5. East longitude area to explore
6. Parameter to extract (see on tkdegrib.exe which value to choose)
7. Folder where save the files
8. Start point Latitude
9. Start point Longitude
10. Finish point Latitude
11. Finish point Longitude

Example:
```
SkymapPathfinder.exe 030 48 40 10 20 25 C:\Skymap 46 11 42 19.5
```
In the example above, the weather forecast requested is 30 hours from now with an area bounded to north 48, south 40, west 10 and east 20 degrees, taking only the parameter 25 (high level cloud), the working directory where stores files is "C:\Skymap", with a start point in 46 N 11 E and a finish point in 42 N 19.5 E

### Results
The final path will be saved in .csv file, in each rows is stored the Latitude and Longitude of the point. The result is displayed below:

<p align="center">
  <img src= "https://github.com/MatteoM95/Skymap-path-planner/blob/main/Media/path.gif" width="350" height="350" />
</p>
