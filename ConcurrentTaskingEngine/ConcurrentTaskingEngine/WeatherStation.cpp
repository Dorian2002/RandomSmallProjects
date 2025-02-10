#include <random>
#include <thread>
#include <chrono>
#include "WeatherStation.h"
#include <iostream>

std::mutex WeatherStation::m_mutex;

WeatherData* WeatherStation::SimulateWeatherStation() {

    // Generate random numbers
    static std::random_device rd;
    static std::mt19937 gen(rd());

    // Temperature distribution (between -10°C and 40°C)
    std::uniform_real_distribution<> temp_dist(-10.0, 40.0);

    // Humidity distribution (between 0% and 100%)
    std::uniform_real_distribution<> humidity_dist(0.0, 100.0);

    // Delay distribution (between 100ms and 2000ms)
    std::uniform_int_distribution<> delay_dist(100, 2000);

    // Generate data
    WeatherData* data = new WeatherData;
    data->m_temperature = temp_dist(gen);
    data->m_humidity = humidity_dist(gen);

    // Simulate delay
    int delay = delay_dist(gen);

    // Lock mutex for thread safe cout
    m_mutex.lock();
    std::cout << "Thread " << std::this_thread::get_id() << " will send data in :" << delay << " ms."<<std::endl;
    // Unlock mutex
    m_mutex.unlock();

    // Put the thread to sleep to simulate a latent return
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));

    return data;
}
