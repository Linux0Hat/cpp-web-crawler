#include "asyncRunner.h"

AsyncRunner::AsyncRunner(std::function<void()> func, int numExecutions, int numThreads)
    : m_function(func), m_numExecutions(numExecutions), m_numThreads(numThreads), m_running(false) {}

AsyncRunner::~AsyncRunner() {
    stop(); 
}

void AsyncRunner::start() {
    if (!m_running) {
        m_running = true;
        for (int i = 0; i < m_numThreads; ++i) {
            m_workers.emplace_back(&AsyncRunner::executionLoop, this);
        }
    }
}

void AsyncRunner::stop() {
    if (m_running) {
        m_running = false;
        for (auto& worker : m_workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        m_workers.clear(); 
    }
}

void AsyncRunner::executionLoop() {
    int executedCount = 0;

    while (m_running) {
        m_function();

        executedCount++;

        if (m_numExecutions > 0 && executedCount >= m_numExecutions) {
            break;
        }
    }
}