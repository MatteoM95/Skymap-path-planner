//https://stackoverflow.com/questions/50013204/jsoncpp-to-parse-from-url		json parsing
//https://github.com/gabrieledarrigo/c-weather									complete c code
//https://gist.github.com/connormanning/41efa6075515019e499c					only for another reference 

#include "WeatherAPI.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>

#define DEBUG 0

WeatherAPI::WeatherAPI() { }

/*
Function that store the json from internet -> see the first link up
@Param1: data from internet
@Param2: dimension of data
@Param3: number of data
@Param4: where to store the data -> return by reference, given in second function
*/
size_t write_to_buffer(void* data, size_t size, size_t nmemb, void* pointer) {
	
	((std::string*)pointer)->append((char*)data, size * nmemb);
	return size * nmemb;
};

/*
Function that create url where to get the json
@Param1: latitude
@Param2: longitude
*/
void WeatherAPI::createUrl(float latitude, float longitude) {
	char urlBase[] = "https://api.openweathermap.org/data/2.5/weather?"; //url di base
	char apiKEY[] = "fcdd8e81ee0817fe242bbe14addb8262"; //key from registration
	char metric[] = "&units=metric"; //not imperial
	char urlJson[200];

	//creation of url
	sprintf(urlJson, "%slat=%.2f&lon=%.2f&appid=%s%s\0", urlBase, latitude, longitude, apiKEY, metric);
	if (DEBUG) {
		printf("%s\n\n", urlJson);
	}
	getJSONFromCURL(urlJson);
}

/*
Fuction that create the string json (not parsed) from a given url -> for debug use google chorme
@Param1: url address where to get JSON
*/
void WeatherAPI::getJSONFromCURL(char url[]) {

	//char url[] = "https://samples.openweathermap.org/data/2.5/weather?lat=35&lon=139&appid=b6907d289e10d714a6e88b30761fae22";
	CURL* myHandle;
	CURLcode result;
	std::string readBuffer;

	myHandle = curl_easy_init();
	if (myHandle) {
		//Function from curl.h -> documentation on https://curl.haxx.se/
		curl_easy_setopt(myHandle, CURLOPT_URL, url);
		curl_easy_setopt(myHandle, CURLOPT_WRITEFUNCTION, write_to_buffer);
		curl_easy_setopt(myHandle, CURLOPT_WRITEDATA, &readBuffer);

		//check that every is ok
		result = curl_easy_perform(myHandle);
		if (result != CURLE_OK) {
			printf("A problem occurred during the HTTP CALL to:\n\n%s\n\nWith the following error: %s\n\n", url, curl_easy_strerror(result));
			printf("Check internet connection\n");
			exit(1);
		}
		//free - opposite of init
		curl_easy_cleanup(myHandle);
		if (DEBUG) {
			std::cout << readBuffer + "\n" << std::endl;
		}

		//copy the json from string to char* and size
		strcpy(JSONstring, readBuffer.c_str());
		size_String = strlen(JSONstring);
	}
	
	//parse the json
	struct Parsed_Json weather = parse_json(JSONstring, 2048);
	//print the info inside json
	if (DEBUG) {
		print_weather_data(JSONstring, weather);
	}
	setCloudWindCondition(JSONstring, weather);
	
}

/*
Parsing and analyze the JSON string
@Param1: string JSON
@Param2: number of token in JSON
@Return: the stuct with data
*/
struct Parsed_Json WeatherAPI::parse_json(char* data, int number_of_tokens) {
	jsmn_parser parser;
	jsmn.jsmn_init(&parser);
	struct Parsed_Json parsed;

	// Calculate the memory required for the tokens
	size_t size = sizeof(jsmntok_t) * number_of_tokens;
	jsmntok_t* tokens = (jsmntok_t*)malloc(size);

	int result = jsmn.jsmn_parse(&parser, data, strlen(data), tokens, number_of_tokens);

	if (result < 0) {
		printf("Failed to parse JSON");
		perror("Err");
		exit(1);
	}

	if (tokens[0].type != JSMN_ARRAY && tokens[0].type != JSMN_OBJECT) {
		printf("The first element should be an array or an object");
		exit(1);
	}

	parsed.number_of_tokens = result;
	parsed.tokens = tokens;

	return parsed;
}

int WeatherAPI::check_json_string(const char* to_search,const char* string, jsmntok_t token) {
	if (token.type == JSMN_STRING && strcmp(to_search, string) == 0) {
		return 0;
	}
	return -1;
}

void WeatherAPI::print_key_value(const char* format, const char* data, jsmntok_t token) {
	printf(format, token.end - token.start, data + token.start);
}

void WeatherAPI::setCloudWindCondition(char* data, struct Parsed_Json weather) {
	for (int i = 1; i < weather.number_of_tokens; i++) {
		int length = weather.tokens[i].end - weather.tokens[i].start;
		char* string = (char*)malloc((length + 1) * sizeof(char));

		strncpy(string, data + weather.tokens[i].start, length);
		string[length] = '\0';
		char tmp[100];

		//CLOUD
		if (check_json_string("all", string, weather.tokens[i]) == 0) { //QUI LA FUNZIONE
			//print_key_value("\tClouds: %.*s%%\n", data, weather.tokens[i + 1]);
			sprintf(tmp, "% .*s", weather.tokens[i + 1].end - weather.tokens[i + 1].start, data + weather.tokens[i + 1].start);
			cloud = atoi(tmp);
		}

		//WIND
		if (check_json_string("speed", string, weather.tokens[i]) == 0) { //QUI LA FUNZIONE
			//print_key_value("\tWind speed: %.*s ms2\n", data, weather.tokens[i + 1]);
			sprintf(tmp, "% .*s", weather.tokens[i + 1].end - weather.tokens[i + 1].start, data + weather.tokens[i + 1].start);
			wind = atoi(tmp);
		}
	}
}

int WeatherAPI::getCloudCondition() {
	return cloud;
}

float WeatherAPI::getWindCondition() {
	return wind;
}

/*
Print the weather data from json
@Param1: JSON string
@Param2: struct with stored all data
*/
void WeatherAPI::print_weather_data(char* data, struct Parsed_Json weather) {
	for (int i = 1; i < weather.number_of_tokens; i++) {
		int length = weather.tokens[i].end - weather.tokens[i].start;
		char* string = (char*)malloc((length + 1) * sizeof(char));

		strncpy(string, data + weather.tokens[i].start, length);
		string[length] = '\0';

		if (check_json_string("coord", string, weather.tokens[i]) == 0) {
			printf("Coordinates\n");
		}

		if (check_json_string("lon", string, weather.tokens[i]) == 0) {
			print_key_value("\tLongitude: %.*s \n", data, weather.tokens[i + 1]);
		}

		if (check_json_string("lat", string, weather.tokens[i]) == 0) {
			print_key_value("\tLatitude: %.*s \n", data, weather.tokens[i + 1]);
		}

		if (check_json_string("weather", string, weather.tokens[i]) == 0) {
			printf("Weather\n");
		}

		//CLOUDS
		if (check_json_string("clouds", string, weather.tokens[i]) == 0) { //QUI LA FUNZIONE
			//print_key_value("\tClouds: %.*s%%\n", data, weather.tokens[i + 1]);
		}
		if (check_json_string("all", string, weather.tokens[i]) == 0) { //QUI LA FUNZIONE
			print_key_value("\tClouds: %.*s%%\n", data, weather.tokens[i + 1]);
		}

		//VENTO
		if (check_json_string("wind", string, weather.tokens[i]) == 0) { //QUI LA FUNZIONE
			//print_key_value("\tClouds: %.*s%%\n", data, weather.tokens[i + 1]);
		}
		if (check_json_string("speed", string, weather.tokens[i]) == 0) { //QUI LA FUNZIONE
			print_key_value("\tWind speed: %.*s ms2\n", data, weather.tokens[i + 1]);
		}

		if (check_json_string("main", string, weather.tokens[i]) == 0 && weather.tokens[i + 1].type != JSMN_OBJECT) {
			print_key_value("\tConditions: %.*s ", data, weather.tokens[i + 1]);
		}

		if (check_json_string("temp", string, weather.tokens[i]) == 0 && weather.tokens[i + 1].type != JSMN_OBJECT) {
			print_key_value("\n\tTemperature: %.*s Celsius\n", data, weather.tokens[i + 1]);
		}

		if (check_json_string("pressure", string, weather.tokens[i]) == 0 && weather.tokens[i + 1].type != JSMN_OBJECT) {
			print_key_value("\tPressure: %.*s hPa\n", data, weather.tokens[i + 1]);
		}

		if (check_json_string("humidity", string, weather.tokens[i]) == 0 && weather.tokens[i + 1].type != JSMN_OBJECT) {
			print_key_value("\tHumidity: %.*s%%\n", data, weather.tokens[i + 1]);
		}
	}

	printf("\n--------------------------------------------------------\n\n");
}
