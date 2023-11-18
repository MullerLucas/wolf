#pragma once

#include "../config.h"
#include "../io/writer.h"
#include "../io/reader.h"

namespace wolf {

// ----------------------------------------------

class Runner {
public:
    bool should_run_;

    Runner(const Config& config);
    virtual ~Runner();

    Runner(const Runner&) = delete;
	Runner& operator=(const Runner&) = delete;
	Runner(Runner&&) = delete;
	Runner& operator=(Runner&&) = delete;

    static std::unique_ptr<Runner> create(const Config& config);

    virtual void setup() = 0;
    virtual void run() = 0;

protected:
    const Config& config_;
    Reader* reader_;
    Writer* writer_;
};

// ----------------------------------------------

}
