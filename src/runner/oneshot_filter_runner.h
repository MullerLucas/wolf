#pragma once

#include <string>
#include <vector>

#include "runner.h"
#include "../filter/multi_trie_filter.h"

namespace wolf {

class OneShotFilterRunner : public Runner {
public:
    OneShotFilterRunner(const Config *config);

    void run() override;
};

}
