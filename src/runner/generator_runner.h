#pragma once

#include <string>
#include <vector>

#include "runner.h"

namespace wolf {

// ----------------------------------------------

class GeneratorRunner : public Runner {
public:
    GeneratorRunner(const Config& config);

    void setup() override;
    void run() override;

private:
    std::vector<std::string> input_;
};

// ----------------------------------------------

}
