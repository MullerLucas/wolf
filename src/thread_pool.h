#pragma once

#include <functional>
#include <future>
#include <queue>
#include <thread>
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
	ThreadPool &operator=(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool &operator=(ThreadPool&&) = delete;

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

}
