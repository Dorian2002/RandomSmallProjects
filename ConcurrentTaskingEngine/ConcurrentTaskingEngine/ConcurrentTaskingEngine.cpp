#include <iostream>
#include <thread>
#include "WeatherStation.h"
#include "WeatherMonitor.h"

// Method to add as much random data as the passed count
void AddRandomData(WeatherMonitor& monitor, int count) {
    for (int i = 0; i < count; ++i) {
        // Simulate a weather station data then add them to the weather monitor
        WeatherData* data = WeatherStation::SimulateWeatherStation();
        monitor.AddData(data);
    }
}

int main() {
    // Get the start time to calculate total execution time at the end
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    // Create the weather monitor
    WeatherMonitor monitor;

    // Simple test
    WeatherData* data1 = new WeatherData{ 20.0, 50.0 };
    WeatherData* data2= new WeatherData{ 25.0, 60.0 };
    monitor.AddData(data1);
    monitor.AddData(data2);

    std::pair<double, double>* averages = monitor.CalculateAverages();
    std::cout << "Average Temperature: " << averages->first << ", Average Humidity: " << averages->second << std::endl;

    // Multi-threaded test
    std::vector<std::thread> threads;
    // Loop as much as we want thread, 5 here
    for (int i = 0; i < 5; ++i) {
        // Create thread by passing method adress and parameters
        threads.emplace_back(AddRandomData, std::ref(monitor), 10);
    }

    // Loop on all thread to wait them before continue execition
    for (auto& t : threads) {
        t.join();
    }

    // Once all threads ends calculate the average data of weather monitor
    averages = monitor.CalculateAverages();
    // Print averages
    std::cout << "After multi-threading - Average Temperature: " << averages->first << ", Average Humidity: " << averages->second << std::endl;

    // Get the end time
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //Print the time elapsed between start and end of execution
    std::cout << std::endl << "Time to get data : " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

    // Call the method that delete all pointers in data vector from weather monitor
    monitor.DeleteData();
    // Delete the averages var
    delete averages;

    return 0;
}

