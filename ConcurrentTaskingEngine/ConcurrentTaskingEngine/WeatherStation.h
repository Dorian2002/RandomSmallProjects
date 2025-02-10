#pragma once
#include <mutex>

struct WeatherData
{
	double m_temperature;
	double m_humidity;
};

class WeatherStation
{
	// Static mutex because the method that uses it is static too
	static std::mutex m_mutex;
public:
	static WeatherData* SimulateWeatherStation();
};

