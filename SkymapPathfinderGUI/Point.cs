using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SkymapPathfinderGUI { 

    public class Point {

        private float cloudPercentage;
        private float wind; //to be implement
        private float latitude;
        private float longitude;
        private int x;
        private int y;
        public Point(){}

        public Point(float cl, float lat, float lon, int x_, int y_){
            this.cloudPercentage = cl;
            this.latitude = lat;
            this.longitude = lon;
            this.x = x_;
            this.y = y_;
        }

        public int getX() {
            return x;
        }
        public int getY() {
            return y;
        }
        public float getLatitude() {
            return latitude;
        }
        public float getLongitude() {
            return longitude;
        }
        public float getCloudPercentage() {
            return cloudPercentage;
        }
        public float getWind() {
            return wind;
        }

        public void setX(int x_) {
            this.x = x_;
        }

        public void setY(int y_) {
            this.y = y_;
        }

        public void setLatitude(float lat) {
            this.latitude = lat;
        }

        public void setLongitude(float lon) {
            this.longitude = lon;
        }

        public void setCloudPercentage(float cloud) {
            this.cloudPercentage = cloud;
        }

        public void stampa() {
            Console.WriteLine("Punto:  -   X: {0}, Y: {1}, lat: {2}, lon: {3}, cloud: {4}", x, y, latitude, longitude, cloudPercentage);
        }

        public string getStampa() {
            string str = "Punto:  -   X: " + this.x +", Y: " + this.y + ", lat: " + this.latitude + ", lon: " + this.longitude + ", cloud: " + this.cloudPercentage;
            return str;
        }
}
}
