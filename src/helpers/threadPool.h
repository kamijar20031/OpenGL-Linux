#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <functional>
#include <vector>
#include <thread>
#include <cstdint>
#include <condition_variable>
#include <queue>

class ThreadPool
{
    using Task = std::function<void()>;
    
    std::vector<std::thread> threads;
    std::condition_variable cv;
    bool stopThreads = false;
    std::mutex mut;
    std::queue <Task> tasks;

    void start (uint8_t numThreads);
public:
    void stop();
    ThreadPool() {};
    ThreadPool(uint8_t numThreads);
    ~ThreadPool();
    void enqueue(Task task);
        
    ThreadPool& operator=(ThreadPool&& other) {
        if (!threads.empty() || stopThreads || !tasks.empty()) {
            throw std::runtime_error("ThreadPool must be empty to assign new value");
        }

        threads = std::move(other.threads);
        tasks = std::move(other.tasks);
        stopThreads = other.stopThreads;
        return *this;
    }
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
};

#endif