#pragma once

#include <chrono>

#include <QDebug>

namespace fl::utils {

    class Bechmark {
    public:
        Bechmark() 
        {
            beginBench = std::chrono::high_resolution_clock::now();
        }
        ~Bechmark() 
        {
            Stop();
        }

        void Stop() 
        {
            auto endTimePoint = std::chrono::high_resolution_clock::now();

            auto start = std::chrono::time_point_cast <std::chrono::microseconds>(beginBench).time_since_epoch().count();
            auto end = std::chrono::time_point_cast <std::chrono::microseconds>(endTimePoint).time_since_epoch().count();
            
            auto duration = end - start;
            double ms = duration * 0.001;

            qDebug() << duration << "us (" << ms << "ms)";
        }

    private:
        std::chrono::time_point <std::chrono::high_resolution_clock> beginBench;
    };
}