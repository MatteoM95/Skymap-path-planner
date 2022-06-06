#include "Point.h"
#include <iostream>
#include <windows.h>

using namespace std;

Point::Point(){}

Point::Point(int cl, float lat, float lon, int x_, int y_){
	this->cloudPercentage = cl;
	this->latitude = lat;
	this->longitude = lon;
	this->x = x_;
	this->y = y_;
}

int Point::getX() {
	return x;
}
int Point::getY() {
	return y;
}
float Point::getLatitude() {
	return latitude;
}
float Point::getLongitude() {
	return longitude;
}
int Point::getCloudPercentage() {
	return cloudPercentage;
}
float Point::getWind() {
	return wind;
}

void Point::setX(int x_) {
	this->x = x_;
}

void Point::setY(int y_) {
	this->y = y_;
}

void Point::setLatitude(float lat) {
	this->latitude = lat;
}

void Point::setLongitude(float lon) {
	this->longitude = lon;
}

void Point::setCloudPercentage(int cloud) {
	this->cloudPercentage = cloud;
}

void Point::stampa() {
	std::cout << "X: " << x << ", Y: " << y << ", lat: " << latitude << ", lon: " << longitude << ", cloud:" << cloudPercentage << endl;
}