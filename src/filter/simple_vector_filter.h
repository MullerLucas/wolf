#pragma once

#include <string>
#include <vector>

#include "../utils.h"
#include "../thread_pool.h"

namespace wolf {

// ----------------------------------------------

struct SVFSession {
    std::vector<const std::string*> filtered;
    std::string                     prefix      = "";
    usize                           depth       = 0;
};

// ----------------------------------------------

class SimpleVectorFilter {
public:
    SimpleVectorFilter(usize thread_count);

    SVFSession create_session(const std::vector<std::string> &unfiltered) const;

    void push(SVFSession &session, const std::string &prefix);
    void collect(SVFSession &session) const;

private:
    static constexpr usize WORKLOAD_MULTIPLIER = 2;

    const usize thread_count_;
    ThreadPool  pool_;

    static void push_slice(std::vector<const std::string*> &filtered,
                           usize start, usize end, const std::string &prefix,
                           usize offset);
};

// ----------------------------------------------

}
