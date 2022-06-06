#pragma once
class Point {
public:
	
	Point();
	Point( int cl, float lat, float lon, int x_, int y_ );
	int getX();
	int getY();
	float getLatitude();
	float getLongitude();
	int getCloudPercentage();
	float getWind();

	void setX(int x);
	void setY(int y);
	void setLatitude(float lat);
	void setLongitude(float lon);
	void setCloudPercentage(int cloud);

	void stampa();

private:
	int cloudPercentage;
	float wind; //to be implement
	float latitude;
	float longitude;
	int x;
	int y;
};

