#pragma once

#include <memory>

#include "filter/filter.h"
#include "io/reader.h"
#include "io/writer.h"

namespace wolf {

// ----------------------------------------------

class App {
public:
    App(
        std::unique_ptr<Reader> reader,
        std::unique_ptr<Writer> writer,
        std::unique_ptr<Filter> filter,
        std::string             prefix
    );

private:
    std::unique_ptr<Reader> reader_;
    std::unique_ptr<Writer> writer_;
    std::unique_ptr<Filter> filter_;
    std::string             prefix_;
};

// ----------------------------------------------

};

