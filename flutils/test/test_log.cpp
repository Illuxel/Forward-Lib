#include "fl/utils/Log.hpp"
using namespace Forward;

#include <gtest/gtest.h>

#include <iostream>
#include <thread>
#include <future>

TEST(Logger, Output) 
{
    auto logger = Log::Get("test_log");

    logger->SetOutputMode(Logger::OutputFlag::Console);

    // Start a thread to log messages
    std::thread logThread([&logger]() {
        for (int i = 0; i < 10; ++i) {
            logger->Log(Logger::Level::INFO, "Func", "Log message ");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    // Start an async operation to log a message
    std::async([&logger]() {
        logger->Log(Logger::Level::INFO, "Func", "Async log message");
    });

    // Wait for the thread to finish
    logThread.join();
}