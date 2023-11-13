#pragma once

#include "../utils.h"
#include <string>
#include <vector>

namespace wolf {

class SimpleFilter {
public:
    std::vector<const std::string*> filter(
        const std::string& pattern,
        const std::vector<std::string>& input,
        usize num_threads
    );
private:
    static void process_workload(
        const std::string& pattern,
        const std::vector<std::string>& input,
        std::vector<const std::string*>& output,
        usize start,
        usize end
    );
};

}
