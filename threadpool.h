#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <memory>

using Task = std::function<void()>;
using InputTask = std::function<bool()>;

class ThreadPool
{
public:
	ThreadPool() = default;

	ThreadPool(std::size_t countThreads)
	{
		start(countThreads);
	}

	~ThreadPool()
	{
		stop();
	}

	void start(std::size_t countThreads)
	{
		for (std::size_t i = 0; i < countThreads; ++i)
		{
			threads.emplace_back([=]()
			{
				while (true)
				{
					Task task;
					{
						std::unique_lock<std::mutex> lock(eventMutex);

						eventVar.wait(lock, [=]() { return stopping || !tasks.empty(); });
	
						if (stopping)
							return;

						task = tasks.front();

						tasks.pop();
					}

					task();
				}
			});
		}
	}

	void stop()
	{
		stopping = true;
		eventVar.notify_all();
		for (auto& thread : threads)
			thread.join();
	}

    std::future<bool> addTask(InputTask task)
	{
        auto wrapper = std::make_shared<std::packaged_task<bool()>>(std::move(task));
        {
            std::unique_lock<std::mutex> lock(eventMutex);
            tasks.push([=]()
            {
                (*wrapper)();
            });
		    eventVar.notify_one();
        }
        return wrapper->get_future();
	}

private:
	std::vector<std::thread> threads;
    std::queue<Task> tasks;
	std::mutex eventMutex;
	std::condition_variable eventVar;
	bool stopping = false;
};

#endif //THREADPOOL_H
