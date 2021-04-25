//#ifndef THREADPOOL_H
//#define THREADPOOL_H

//#include <vector>
//#include <future>
//#include <thread>
//#include <vector>
//#include <unordered_map>
//#include <condition_variable>
//#include <queue>
//#include <mutex>
//#include <atomic>
//#include <memory>
//#include <unordered_map>
//#include <array>

//struct Pixel_s
//{
//    unsigned char r;
//    unsigned char g;
//    unsigned char b;
//};

//struct ThreadData_s
//{
//    Pixel_s* m_Data;
//    std::size_t m_Len;
//};

//using Task = std::function<void(std::unordered_map<std::thread::id, ThreadData_s>&, const std::array<int, 255>&)>;

//class ThreadPool
//{
//public:
//    ThreadPool() = default;

//    explicit ThreadPool(std::size_t in_CountThreads)
//    {
//        start(in_CountThreads);
//    }


//    ~ThreadPool()
//    {
//        stop();
//    }

//    void start(std::size_t in_CountThreads)
//    {
//        for (std::size_t i = 0; i < in_CountThreads; ++i)
//        {
//            m_ThreadsPool.emplace_back([=](){
//                while (true)
//                {
//                    std::function<void()> task;
//                    {
//                        std::unique_lock<std::mutex> lock(m_EventMutex);
//                        m_EventVar.wait(lock, [=](){ return m_Stopping.load() || !m_Tasks.empty(); });

//                        if (m_Stopping.load())
//                            return;

//                        task = m_Tasks.front();
//                        m_Tasks.pop();
//                    }

//                    task();
//                    ++m_CountDone;
//                }
//            });
//        }
//    }

//    void stop()
//    {
//        m_Stopping.store(true);
//        m_EventVar.notify_all();
//    };

//    void addTask(Task in_Task,
//    std::unordered_map<std::thread::id, ThreadData_s>& in_Map,
//    const std::array<int, 255>& in_Array)
//    {
//        //auto wrapper = std::make_shared<std::packaged_task<Task>>(std::move(in_Task), std::ref(in_Map), std::ref(in_Array));
//        //auto wrapper = std::packaged_task<Task>(std::move(in_Task), std::ref(in_Map), std::ref(in_Array));
//        {
//            std::unique_lock<std::mutex> lock(m_EventMutex);

//            m_Tasks.emplace([&](){
//                in_Task(in_Map, in_Array);
//            });
//        }

//        m_EventVar.notify_one();
//    }

//    bool IsDoneCountTasks(std::size_t in_Count)
//    {
//        return in_Count == static_cast<std::size_t>(m_CountDone);
//    }

//    std::size_t GetThreadsCount() const
//    {
//        return m_ThreadsPool.size();
//    }

//private:
//    std::vector<std::thread> m_ThreadsPool;
//    std::queue<void()> m_Tasks;
//    std::condition_variable m_EventVar;
//    std::mutex m_EventMutex;
//    std::atomic<bool> m_Stopping = false;
//    std::atomic<int> m_CountDone = 0;
//};

//#endif // THREADPOOL_H
