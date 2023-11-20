#pragma once

#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "utils.h"

namespace wolf {

// ----------------------------------------------

class ThreadPool {
public:
    using Future = std::future<void>;

    explicit ThreadPool(usize thread_count);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

    std::future<void> enqueue(std::function<void()> f);

    static std::vector<Future> create_futures(usize thread_count);
    static void await_futures(std::vector<Future> &futures);

private:
    bool                                should_terminate_;
    std::vector<std::thread>            threads_;
    std::queue<std::function<void()>>   tasks_;
    std::mutex                          queue_mutex_;
    std::condition_variable             mutex_cond_;
};

// ----------------------------------------------

inline ThreadPool::ThreadPool(usize thread_count)
    : should_terminate_(false)
{
    for (usize i = 0; i < thread_count; ++i) {
        threads_.emplace_back([this] {
            while (true) {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(queue_mutex_);
                    mutex_cond_.wait(lock, [this] {
                        return should_terminate_ || !tasks_.empty();
                    });

                    if (should_terminate_ && tasks_.empty()) return;

                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task();
            }
        });
    }
}

inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        should_terminate_ = true;
    }
    mutex_cond_.notify_all();

    for (std::thread &thread : threads_)
        thread.join();
}

inline std::future<void> ThreadPool::enqueue(std::function<void()> f) {
    auto task = std::make_shared<std::packaged_task<void()>>(std::move(f));
    std::future<void> future = task->get_future();

    {
        std::unique_lock<std::mutex> lock(queue_mutex_);

        if (should_terminate_)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks_.emplace([task = std::move(task)]() mutable {
            (*task)();
        });
    }

    mutex_cond_.notify_one();
    return future;
}

inline std::vector<ThreadPool::Future> ThreadPool::create_futures(usize thread_count)
{
    std::vector<ThreadPool::Future> futures{};
    futures.reserve(thread_count);
    return futures;
}

inline void ThreadPool::await_futures(std::vector<ThreadPool::Future> &futures)
{
    for (auto& future : futures)
        future.wait();
}

}
