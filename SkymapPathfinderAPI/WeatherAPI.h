#ifndef WEATHER_API
#define WEATHER_API

#include <curl/curl.h>
#include "jsmnImplemented.h"

struct Parsed_Json {
	int number_of_tokens;
	jsmntok_t* tokens;
};

class WeatherAPI {
	public:
		//FUNCTIONS
		WeatherAPI();
		void getJSONFromCURL(char url[]);
		void getInfoFromJSON();
		struct Parsed_Json parse_json(char* data, int number_of_tokens);
		//size_t write_to_buffer(void* data, size_t size, size_t nmemb, void* pointer);
		int check_json_string(const char* to_search, const char* string, jsmntok_t token);
		void print_key_value(const char* format, const char* data, jsmntok_t token);
		void print_weather_data(char* data, struct Parsed_Json weather);
		void createUrl(float latitude, float longitude);
		void setCloudWindCondition(char* data, struct Parsed_Json weather);
		int getCloudCondition();
		float getWindCondition();

		//VARIABLE
		int cloud = 0;
		float wind = 0.0;
		char JSONstring[5000];
		int size_String = 0;
		jsmnImplemented jsmn;

	private:

};

#endif