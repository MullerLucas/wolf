#include "thread_pool.h"
#include "utils.h"
#include <mutex>
#include <thread>

namespace wolf {

void ThreadPool::start(usize num_threads) {
    for (usize i = 0; i < num_threads; i++) {
        threads_.emplace_back(std::thread(&ThreadPool::thread_loop, this));
    }
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        should_terminate_ = true;
    }

    mutex_cond_.notify_all();

    for (std::thread& thread : threads_) {
        thread.join();
    }

    threads_.clear();
}

void ThreadPool::queue_job(const Job& job) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        jobs_.push(job);
    }
    mutex_cond_.notify_one();
}

bool ThreadPool::is_busy() {
    bool pool_is_busy = false;
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        pool_is_busy = !jobs_.empty();
    }
    return pool_is_busy;
}

void ThreadPool::thread_loop() {
    while (true) {
        Job job;

        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            mutex_cond_.wait(lock, [this] {
                return !jobs_.empty() || should_terminate_;
            });

            if (jobs_.empty() && should_terminate_) {
                return;
            }

            job = jobs_.front();
            jobs_.pop();
        }

        job();
    }
}

}
