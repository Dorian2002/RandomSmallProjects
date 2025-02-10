#include "WeatherMonitor.h"
#include "WeatherStation.h"
#include <numeric>

void WeatherMonitor::AddData(WeatherData* _data)
{
	// Lock the mutex so the vector is thread safe
	m_mutex.lock();
	m_data.push_back(_data);
	// Unlock the mutex so other thread can write in the vector
	m_mutex.unlock();
}

std::pair<double, double>* WeatherMonitor::CalculateAverages() const
{
	// Instantiate the pair to return
	std::pair<double, double>* toReturn = new std::pair<double, double>();

	// Check if the vector ins't empty
	if (!m_data.empty()) {
		double tempTotal = 0;
		double humiTotal = 0;

		// Loop on data and accumulate them
		for (WeatherData* dw : m_data) {
			tempTotal += dw->m_temperature;
			humiTotal += dw->m_humidity;
		}

		// Put averages in return var by dividing Totals by data number
		toReturn->first = tempTotal / m_data.size();
		toReturn->second = humiTotal / m_data.size();
		return toReturn;
	}
	// Set absurd values if no data
	toReturn->first = 999999999;
	toReturn->second = 999999999;
	return toReturn;
}

void WeatherMonitor::DeleteData()
{
	// As all WeatherData are pointers, we must delete them
	for (WeatherData* wd : m_data) {
		delete wd;
	}
}
