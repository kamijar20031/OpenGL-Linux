#include "threadPool.h"

void ThreadPool::start(uint8_t numThreads)
{
    for (uint8_t i = 0; i<numThreads; i++)
    {
        threads.emplace_back([this] {
            while (true)
            {
                Task task;
                {
                    std::unique_lock<std::mutex> lock(mut);
                    cv.wait(lock, [&] {return stopThreads || !tasks.empty();});
                    if (stopThreads && tasks.empty())
                        break;
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();                   
            }
        });
    }
}
void ThreadPool::stop()
{
    {
        std::unique_lock<std::mutex> lock(mut);
        stopThreads=true;
    }
    cv.notify_all();
    for (auto &t: threads)
        t.join();
}

ThreadPool::ThreadPool(uint8_t numThreads)
{
    start(numThreads);
}

ThreadPool::~ThreadPool()
{
    if (!stopThreads)
        stop();
}

void ThreadPool::enqueue(Task task)
{
    {
        std::unique_lock<std::mutex>(mut);
        tasks.emplace(std::move(task));
    }   
    cv.notify_one();
}