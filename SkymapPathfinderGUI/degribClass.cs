/*
Class aim: conversion grib file and extraction weather data
*/

using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.VisualBasic.FileIO;

namespace SkymapPathfinderGUI {
    public class degribClass {

        static int MAX_DIM = 200;
        int cols = 0;
        int rows = 0;
        Point[,] matrix;
        public degribClass(){}

        //Conversion of grib file and extraction of useful data 
        public void convertToCSV(string gribName, string path, int param, string csvFilename) {

            string command, commandExe, commandParam;
            //command = path_t + "\\degrib.exe " + path_t + "\\" + gribName_t + " -C -msg " + param_t + " -Csv -out " + csv_t;
            command = path + "\\degrib.exe " + gribName + " -C -msg " + param + " -Csv -out " + csvFilename;
            commandExe = path + "\\degrib.exe";
            commandParam = gribName + " -C -msg " + param + " -Csv -out " + csvFilename;
            Console.WriteLine("\n" + command + "\n");
            
            System.Diagnostics.Process.Start(path + "\\degrib.exe", commandParam);

        }

        //Allocation of cloud matrix with data just downloaded
        public Point[,] readCSV(string path, string fileName) {
        
            matrix = new Point[MAX_DIM, MAX_DIM];

            string path_t = path + "\\" + fileName;

            Console.WriteLine("Path CSV:    " + path_t);

            int x_, y_, maxRow = 0, maxCol = 0;
            float lat, lon;
            float cloudPer;

                try {
                    StreamReader readFile = new StreamReader(path_t);
                    string line;
                    string[] row;
                    readFile.ReadLine();
                    while ((line = readFile.ReadLine()) != null)
                    {
                        row = line.Split(',');
                        x_ = int.Parse(row[0]);
                        if (x_ > maxCol) { maxCol = x_; }
                        y_ = int.Parse(row[1]);
                        if (y_ > maxRow) { maxRow = y_; }
                        lat = float.Parse(row[2], CultureInfo.InvariantCulture.NumberFormat);
                        lon = float.Parse(row[3], CultureInfo.InvariantCulture.NumberFormat);
                        cloudPer = float.Parse(row[4], CultureInfo.InvariantCulture.NumberFormat);

                        Point p = new Point(cloudPer, lat, lon, x_, y_);
                        matrix[y_ - 1, x_ - 1] = p;
                        //matrix[y_ - 1, x_ - 1].stampa();
                    }
                    readFile.Close();
                } catch (Exception e) {
                    Console.WriteLine("File " + fileName + " not found\n");
                }

            rows = maxRow;
            cols = maxCol;
            
            //inversione della matrice per creare piano XY
            int tmp = maxRow - 1;
            Point[,] tmpMatrix = new Point[MAX_DIM, MAX_DIM];

                for (int i = 0; i < maxRow; i++) {
                    for (int j = 0; j < maxCol; j++) {
                        tmpMatrix[tmp, j] = matrix[i, j];
                    }
                    tmp--;
                }

                /*
                //Stampa
                for (int i = 0; i < rows; i++) {
                     for (int j = 0; j < cols; j++) {
                            Console.Write(matrix[i, j].getCloudPercentage() + " ");
                     }
                     Console.Write("\n");
                }
                */

                return tmpMatrix;
            }

        public int getRows() {
            return this.rows;
        }

        public int getCols() {
            return this.cols;
        }

    }
}
