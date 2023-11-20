#pragma once

#include <string>
#include <vector>

#include "runner.h"
#include "../filter/multi_trie_filter.h"
#include "../gui/filter_window.h"

namespace wolf {

// ----------------------------------------------

class IncrementalFilterRunner : public Runner {
public:
    IncrementalFilterRunner(const Config *config);

    void run() override;

private:
    std::vector<std::string>    input_;
    FilterWindow                window_;
    FilterWindowState           state_;
    bool                        should_close_   = false;

    bool handle_input_changed(const char *input);
};

// ----------------------------------------------

}
