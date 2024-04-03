#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <thread>

#define THREAD_COUNT 8
#define MINUTES 60
#define HOURS 240

std::mutex mutex;

/// Checks if all sensors have finished reading
bool sensorsReady(int caller, std::vector<bool> &sensors)
{
    std::lock_guard<std::mutex> lock(mutex);
    for (int i = 0; i < (int)sensors.size(); i++)
    {
        if (!sensors[i] && caller != i)
        {
            return false;
        }
    }
    return true;
}

/// Prints the largest difference between two temperatures in the hour's readings
void largestDifference(std::vector<int> &readings)
{
    int step = 10, interval = 0, maxDiff = 0;

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        int offset = i * MINUTES;

        for (int j = 0; j < MINUTES - step + 1; j++)
        {
            int max = *std::max_element(readings.begin() + offset + j, readings.begin() + offset + j + step);
            int min = *std::min_element(readings.begin() + offset + j, readings.begin() + offset + j + step);
            int diff = max - min;

            if (diff > maxDiff)
            {
                maxDiff = diff;
                interval = j;
            }
        }
    }
    std::cout << "Largest difference: " << maxDiff << "F from minute " << interval << " to " << (interval + 10) << std::endl;
}

/// Prints the five highest temperatures in the hour's readings
void highestTemperatures(std::vector<int> &readings)
{
    std::priority_queue<int, std::vector<int>, std::greater<int>> highestTemps;

    // Find the five highest temperatures
    for (int temp : readings)
    {
        highestTemps.push(temp);
        if (highestTemps.size() > 5)
        {
            highestTemps.pop();
        }
    }

    // Print the five highest temperatures
    std::cout << "Five highest temperatures: ";
    while (!highestTemps.empty())
    {
        std::cout << highestTemps.top() << "F ";
        highestTemps.pop();
    }
    std::cout << std::endl;
}

/// Prints the five lowest temperatures in the hour's readings
void lowestTemperatures(std::vector<int> &readings)
{
    std::priority_queue<int, std::vector<int>, std::less<int>> lowestTemps;

    // Find the five lowest temperatures
    for (int temp : readings)
    {
        lowestTemps.push(temp);
        if (lowestTemps.size() > 5)
        {
            lowestTemps.pop();
        }
    }

    // Print the five lowest temperatures
    std::cout << "Five lowest temperatures: ";
    while (!lowestTemps.empty())
    {
        std::cout << lowestTemps.top() << "F ";
        lowestTemps.pop();
    }
    std::cout << std::endl;
}

/// Prints the report for the hour's readings
void printReport(int hour, std::vector<int> &readings)
{
    std::cout << "Hour " << hour + 1 << std::endl;

    largestDifference(readings);

    std::sort(readings.begin(), readings.end());

    highestTemperatures(readings);
    lowestTemperatures(readings);

    std::cout << std::endl;
}

/// Simulates a sensor thread that reads temperature data
void sensorThread(int id, std::vector<int> &readings, std::vector<bool> &sensorsStatus)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 100);

    // Read temperature data for each minute of the hour
    for (int i = 0; i < HOURS; i++)
    {
        for (int j = 0; j < MINUTES; j++)
        {
            sensorsStatus[id] = false;
            readings[j + id * MINUTES] = dis(gen);
            sensorsStatus[id] = true;

            // Wait for all sensors to finish reading
            while (!sensorsReady(id, sensorsStatus))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        }

        // Print report for the hour's readings
        if (id == 0)
        {
            mutex.lock();
            printReport(i, readings);
            mutex.unlock();
        }
    }
}

int main()
{
    std::vector<int> readings(THREAD_COUNT * MINUTES);
    std::vector<bool> sensorsStatus(THREAD_COUNT);
    std::thread threads[THREAD_COUNT] = {};

    auto start = std::chrono::high_resolution_clock::now();

    // Create thread for each sensor
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        threads[i] = std::thread(sensorThread, i, std::ref(readings), std::ref(sensorsStatus));
    }

    // Wait for all threads to finish
    for (std::thread &t : threads)
    {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}