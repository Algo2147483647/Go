#ifndef GO_AI_THREADPOOL_H
#define GO_AI_THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

using namespace std;

namespace GoAI {
/*
 *  Thread Pool
 */
class ThreadPool {
public:
    ThreadPool() = default;
    ThreadPool(const int numThreads) {
        init(numThreads);
    }
    ~ThreadPool();

    void init(const int numThreads);

    void addThread(function<void()> initalizer);

    template <class F, class... Args>
    auto addTask(F&& f, Args&&... args)
        -> future<typename invoke_result<F, Args...>::type>;

    void wait_all() {
        for (auto&& result : m_taskResults) {
            result.get();
        }
    }

private:
    vector<thread> m_threads;
    queue<function<void()>> m_tasks;
    vector<future<void>> m_taskResults;

    mutex m_mutex;
    condition_variable m_cv;
    bool exit {false};
}; 

/*
 *  Initial
 */
inline void ThreadPool::init(const int numThreads) {
    for (auto i = 0; i < numThreads; i++) {
        addThread([]() {;});
    }
}

/*
 *  add a thread
 */
inline void ThreadPool::addThread(function<void()> initalizer) {
    m_threads.emplace_back([this, initalizer] {
        initalizer();

        while(true) {
            function<void()> task;
            
            {
                unique_lock<mutex> lock(m_mutex);
                m_cv.wait(lock, [this] {return exit || !m_tasks.empty(); });

                if(exit && m_tasks.empty()) 
                    return;

                task = move(m_tasks.front());
                m_tasks.pop();
            }
        }
    });
}

/*
 *  add a task
 */
template <class F, class... Args>
auto ThreadPool::addTask(F&& f, Args&&... args)
-> future<typename invoke_result<F, Args...>::type> {

    auto task = make_shared<packaged_task<typename invoke_result<F, Args...>::type()>>(
        bind(forward<F>(f), forward<Args>(args)...)
        );

    future<typename invoke_result<F, Args...>::type> res = task->get_future();

    {
        unique_lock<mutex> lock(m_mutex);
        m_tasks.emplace([task]() { (*task)(); });
    }

    m_cv.notify_one();

    return res;
}

/*
 *  ~ThreadPool
 */
inline ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        exit = true;
    }

    m_cv.notify_all();

    for (std::thread& worker : m_threads) {
        worker.join();
    }
}

}

#endif