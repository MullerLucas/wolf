#include "simple_filter.h"
#include "../utils.h"
#include <algorithm>
#include <functional>
#include <string>
#include <thread>
#include <vector>

namespace wolf {


// ----------------------------------------------

std::vector<const std::string*> SimpleFilter::filter(
    const std::string& pattern,
    const std::vector<std::string>& input,
    usize num_threads
) {
    std::vector<const std::string*> output(input.size(), nullptr);
    usize chunck_size = input.size() / num_threads;

    log_info("Parallel filter with %d threads and chucks of size %i\n", num_threads, chunck_size);

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    Timer t1;
    for (usize i = 0; i < num_threads; i++) {
        usize start = i * chunck_size;
        usize end = (i == num_threads - 1) ? input.size() : (i + 1) * chunck_size;

        threads.emplace_back(
            SimpleFilter::process_workload,
            std::ref(pattern),
            std::ref(input),
            std::ref(output),
            start,
            end
        );
    }
    t1.stop();

    Timer t2;
    for (auto& thread : threads) {
        thread.join();
    }
    t2.stop();


    Timer t3;
    auto it = std::remove_if(output.begin(), output.end(), [](const std::string* str) {
        return str == nullptr;
    });
    output.erase(it, output.end());
    t3.stop();

    log_info("Parallel filter: %lld, %lld, %lld\n", t1.elapsed_us().count(), t2.elapsed_us().count(), t3.elapsed_us().count());

    return output;
}

void SimpleFilter::process_workload(
    const std::string& pattern,
    const std::vector<std::string>& input,
    std::vector<const std::string*>& output,
    usize start,
    usize end
) {
    for (usize i = start; i < end; i++) {
        if (input[i].find(pattern) == 0) {
            output[i] = &input[i];
        }
    }
}

// ----------------------------------------------

}
