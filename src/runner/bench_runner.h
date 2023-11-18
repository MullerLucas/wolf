#pragma once

#include <string>
#include <vector>

#include "runner.h"

namespace wolf {

// ----------------------------------------------

class BenchRunner : public Runner {
public:
    BenchRunner(const Config& config);

    void setup() override;
    void run() override;

private:
    std::vector<std::string> input_;
};

// ----------------------------------------------

}
