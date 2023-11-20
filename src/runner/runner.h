#pragma once

#include "../config.h"
#include "../io/writer.h"
#include "../io/reader.h"
#include <string>
#include <vector>

namespace wolf {

// ----------------------------------------------

class Runner {
public:
    Runner(const Config *config);
    virtual ~Runner();

    // TODO(lm)
    Runner(const Runner&) = delete;
	Runner &operator=(const Runner&) = delete;
	Runner(Runner&&) = delete;
	Runner &operator=(Runner&&) = delete;

    static std::unique_ptr<Runner> create(const Config *config);

    virtual void run() = 0;

protected:
    // TODO(lm)
    const Config    *config_;
    Reader          *reader_;
    Writer          *writer_;

    std::vector<std::string> read_word_list();
};

// ----------------------------------------------

}
