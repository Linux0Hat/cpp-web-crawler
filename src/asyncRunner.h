#pragma once

#include <functional>
#include <thread>
#include <atomic>

class AsyncRunner {
    public:
        AsyncRunner(std::function<void()> func, int numExecutions = 1, int numThreads = 1);
        ~AsyncRunner();

        void start();
        void stop();

    private:
        void executionLoop();

        std::function<void()> m_function;
        int m_numExecutions;
        int m_numThreads;
        std::atomic<bool> m_running;
        std::vector<std::thread> m_workers;
};