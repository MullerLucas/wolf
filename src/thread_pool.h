#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "utils.h"

namespace wolf {

class ThreadPool {
public:
    using Job = std::function<void()>;

    void start(usize num_threads);
    void stop();
    bool is_busy();
    void queue_job(const Job& job);

private:
    bool should_terminate_ = false;
    std::mutex queue_mutex_;
    std::condition_variable mutex_cond_;
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> jobs_;

    void thread_loop();
};

}
