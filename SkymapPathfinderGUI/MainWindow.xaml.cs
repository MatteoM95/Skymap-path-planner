/*
Gui class: it contains only graphical object for user interface
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Forms;
using System.Globalization;
using System.Threading;
using System.IO;


namespace SkymapPathfinderGUI {
    public partial class MainWindow : Window {

        //Global variables
        Point start = new Point(0, 0, 0, 0, 0);
        Point finish = new Point(0, 0, 0, 0, 0);
        Point[,] cloudMatrix = new Point[1000, 1000];
        Point[] route;
        int rows, cols;

        string folderPath = "C:\\Icarus";  //folder where is stored the application
        //string folderPath = "C:\\Users\\matte\\Desktop\\GribGuiCSharp\\GribGui\\bin\\Debug";
        string fileName = "cloud.grb2";
        string csvFile = "cloud.csv";

        string hourForecast;//= "036"; //forecast hour from now (max 120 = 5 day) - format 3 digit !!!!
        int date = int.Parse(DateTime.Now.ToString("yyyyMMdd"));

        float topLatitude;
        float bottomLatitude;
        float leftLongitude;
        float rightLongitude;
      
        int parameterToExtract = 25; //height in millibar (max 22) see the documentation

        //Constructor
        public MainWindow() {
            InitializeComponent();
            Console.WriteLine("Data: {0}", date);
            this.Title = "Calculate Route";
            this.WindowStartupLocation = WindowStartupLocation.CenterScreen;
            TextBoxFolderPath.AppendText(folderPath);

            //for float dot - commma problem
            System.Globalization.CultureInfo customCulture = (System.Globalization.CultureInfo)System.Threading.Thread.CurrentThread.CurrentCulture.Clone();
            customCulture.NumberFormat.NumberDecimalSeparator = ".";
            System.Threading.Thread.CurrentThread.CurrentCulture = customCulture;
        }

        //Event handler button exit
        private void ExitButton_Click(object sender, RoutedEventArgs e)  {
            Environment.Exit(0);
        }

        //Event handler button folder
        private void ButtonChangeFolder_Click(object sender, RoutedEventArgs e)  {
            TextBoxFolderPath.Text = String.Empty;
            FolderBrowserDialog folderBrowserDialog1 = new FolderBrowserDialog();
            folderBrowserDialog1.ShowDialog();
            folderPath = folderBrowserDialog1.SelectedPath;
            TextBoxFolderPath.AppendText(folderPath);
        }

        //Event handler button go
        private void GoButton_Click(object sender, RoutedEventArgs e)  {

            TextBoxInfo.Text = String.Empty;

            //initilization of variable 
            float latsta_t = float.Parse(TestBoxStartLat.Text.Trim(), CultureInfo.InvariantCulture.NumberFormat);
            float lonsta_t = float.Parse(TestBoxStartLon.Text.Trim(), CultureInfo.InvariantCulture.NumberFormat);

            float latfin_t = float.Parse(TestBoxFinishLat.Text.Trim(), CultureInfo.InvariantCulture.NumberFormat);
            float lonfin_t = float.Parse(TestBoxFinishLon.Text.Trim(), CultureInfo.InvariantCulture.NumberFormat);

            float topLatitude_t = float.Parse(TestBoxTopLatitude.Text.Trim(), CultureInfo.InvariantCulture.NumberFormat);
            float bottomLatitude_t = float.Parse(TestBoxBottomLatitude.Text.Trim(), CultureInfo.InvariantCulture.NumberFormat);
            float leftLongitude_t = float.Parse(TestBoxLeftLongitude.Text.Trim(), CultureInfo.InvariantCulture.NumberFormat);
            float rightLongitude_t = float.Parse(TestBoxRightLongitude.Text.Trim(), CultureInfo.InvariantCulture.NumberFormat);

            float latsta = decimalPointManage(latsta_t);
            float lonsta = decimalPointManage(lonsta_t);
            float latfin = decimalPointManage(latfin_t);
            float lonfin = decimalPointManage(lonfin_t);
            topLatitude = decimalPointManage(topLatitude_t);
            bottomLatitude = decimalPointManage(bottomLatitude_t);
            leftLongitude = decimalPointManage(leftLongitude_t);
            rightLongitude = decimalPointManage(rightLongitude_t);

            hourForecast = TextBoxForecastHour.Text;
            if (hourForecast.Length != 3) {
                TextBoxInfo.AppendText("HourForecast must be 3 digit. MAX 120. Example: 036\n");
                return;
            }

            parameterToExtract = int.Parse(TextBoxParameter.Text);
            if (parameterToExtract > 25) {
                TextBoxInfo.AppendText("Max value 25, see documentation\n");
                return;
            }
            
            if (topLatitude < bottomLatitude) {
                TextBoxInfo.AppendText("TopLatitude is lower than bottomLatitude\n");
            } else
            if (leftLongitude > rightLongitude) {
                TextBoxInfo.AppendText("LeftLongitude is higher than rightLongitude\n");
            } else
            if (System.Math.Abs( (topLatitude - bottomLatitude)  * (leftLongitude - rightLongitude) ) > 1000) {
                TextBoxInfo.AppendText("Huge Area of exploration: reduce !!!\n");
            } else 
            if ( (latsta <= topLatitude && latsta >= bottomLatitude) 
                && (latfin <= topLatitude && latfin >= bottomLatitude) 
                && (lonsta <= rightLongitude && lonsta >= leftLongitude) 
                && (lonfin <= rightLongitude && lonfin >= leftLongitude)
                ) {
                TextBoxInfo.AppendText("Execution in progress...\n\n");
                start.setLatitude(latsta);
                start.setLongitude(lonsta);
                finish.setLatitude(latfin);
                finish.setLongitude(lonfin);

                //Starting of calculate the path with all give data
                ExecFunction();

            } else {
               TextBoxInfo.AppendText("Something wrong: Latitude/Longitude out of border\n");
            }
            TextBoxInfo.AppendText("\n\n");
        }

        //execution of processing the shortest path (MAIN function of the thread)
        void ExecFunction() {

            TextBoxInfo.AppendText("Execution in progress...\n\n");
            Thread.Sleep(100);
            gribURLManage gribManage = new gribURLManage();
            degribClass degrib = new degribClass();
            Dijkstra dij = new Dijkstra();

            string URL = gribManage.gribURLcreation(hourForecast, date, topLatitude, bottomLatitude, leftLongitude, rightLongitude);
            Console.WriteLine("Download URL grib {0}: \n", URL);
            TextBoxInfo.AppendText("Download URL grib: " + URL + "\n\n");

            string path = folderPath;
            path = path + "\\" + fileName;

            Console.WriteLine("FolderPath: {0} \n", path);
            TextBoxInfo.AppendText("File location: " + path + "         --      GRIB filename: " + fileName + "\n\n");

            gribManage.downloadGRIB(URL, path);
            Thread.Sleep(500);

            degrib.convertToCSV(fileName, folderPath, parameterToExtract, csvFile);
            Thread.Sleep(500);

            cloudMatrix = degrib.readCSV(folderPath, csvFile);
            this.cols = degrib.getCols();
            this.rows = degrib.getRows();

            //stampa matrice su console
            Console.WriteLine("\n\nSource: " + start.getStampa() + ", destination " + finish.getStampa());
            Console.WriteLine("\n\nCloud Matrix: rows: " + rows + " - cols: " + cols + "\n\n");
            TextBoxInfo.AppendText("\n\nCloud Matrix: rows: " + rows + " - cols: " + cols + "\n");
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    TextBoxInfo.AppendText(cloudMatrix[i, j].getCloudPercentage() + " ");
                    Console.Write(cloudMatrix[i, j].getCloudPercentage() + " ");
                }
                TextBoxInfo.AppendText("\n");
                Console.Write("\n");
            }
            
            //Dijkstra algorithm
            Thread.Sleep(1000);
            route = dij.calculatePath(cloudMatrix, rows, cols, start, finish);

            //print of the path
            TextBoxInfo.AppendText("\n\n Path with " + dij.getLenghtPath() + " points:\n\n");
            Console.Write("\n\n Path with " + dij.getLenghtPath() + " points:\n\n");
            for (int i = 0; i < dij.getLenghtPath(); i++) {
                TextBoxInfo.AppendText(route[i].getStampa());
                TextBoxInfo.AppendText("\n");
                route[i].stampa();
            }

            TextBoxInfo.AppendText("Completed");

            //write all path point on csv file
            FileStream outputFile = new FileStream("trackPosition.csv", FileMode.Append, FileAccess.Write, FileShare.Write);
            for (int i = 0; i < dij.getLenghtPath(); i++) {
                string str = route[i].getLatitude() + ", " + route[i].getLongitude() + "\n";
                Console.Write(str);
                byte[] info = new UTF8Encoding(true).GetBytes(str);
                outputFile.Write(info, 0, str.Length);
            }
            outputFile.Close();
            return;
        }


        public float decimalPointManage(float num) {
            float result = 0;

            float rightcomma = num % 1;
            float leftComma = num - rightcomma;

            if (rightcomma >= 0.75) {
                rightcomma = 0.75f;
            } else if (rightcomma >= 0.50 && rightcomma < 0.75) {
                rightcomma = 0.50f;
            } else if (rightcomma >= 0.25 && rightcomma < 0.50) {
                rightcomma = 0.25f;
            } else {
                rightcomma = 0.0f;
            }

            result = leftComma + rightcomma;

            Console.WriteLine("LeftComma: " + leftComma + " rightcomma: " + rightcomma + " = result: " + result);
            return result;
        }
    }
}
