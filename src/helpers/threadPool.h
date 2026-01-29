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
    
    static std::vector<std::thread> threads;
    static std::condition_variable cv;
    static bool stopThreads;
    static std::mutex mut;
    static std::queue <Task> tasks;

public:
    static void start (uint8_t numThreads);
    static void stop();
    static void enqueue(Task task);
};

#endif