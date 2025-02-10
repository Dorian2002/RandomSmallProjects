#pragma once
#include <vector>
#include <mutex>
class WeatherData;

class WeatherMonitor
{
	std::vector<WeatherData*> m_data;
	std::mutex m_mutex;

public:
	void AddData(WeatherData* _data);
	std::pair<double, double>* CalculateAverages() const;
	void DeleteData();
};

