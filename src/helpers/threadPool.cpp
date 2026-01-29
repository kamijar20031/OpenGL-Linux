#include "threadPool.h"

using Task = std::function<void()>;

std::vector<std::thread> ThreadPool::threads;
std::condition_variable ThreadPool::cv;
bool ThreadPool::stopThreads = false;
std::mutex ThreadPool::mut;
std::queue <Task> ThreadPool::tasks;

void ThreadPool::start(uint8_t numThreads)
{
    for (uint8_t i = 0; i<numThreads; i++)
    {
        ThreadPool::threads.emplace_back([&] {
            while (true)
            {
                Task task;
                {
                    std::unique_lock<std::mutex> lock(ThreadPool::mut);
                    ThreadPool::cv.wait(lock, [&] {return ThreadPool::stopThreads || !ThreadPool::tasks.empty();});
                    if (ThreadPool::stopThreads && ThreadPool::tasks.empty())
                        break;
                    task = std::move(ThreadPool::tasks.front());
                    ThreadPool::tasks.pop();
                }
                task();                   
            }
        });
    }
}
void ThreadPool::stop()
{
    {
        std::unique_lock<std::mutex> lock(ThreadPool::mut);
        ThreadPool::stopThreads=true;
    }
    ThreadPool::cv.notify_all();
    for (auto &t: ThreadPool::threads)
        t.join();
}

void ThreadPool::enqueue(Task task)
{
    {
        std::unique_lock<std::mutex> lock(ThreadPool::mut);
        ThreadPool::tasks.emplace(std::move(task));
    }   
    ThreadPool::cv.notify_one();
}