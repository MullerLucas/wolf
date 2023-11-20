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

    static std::unique_ptr<Runner> create(const Config *config);

    virtual void run() = 0;

protected:
    // TODO(lm)
    const Config    *config_;
    Reader          *reader_;
    Writer          *writer_;

    std::vector<std::string> read_word_list();

    Runner(const Runner&) = default;
	Runner &operator=(const Runner&) = default;
	Runner(Runner&&) = default;
	Runner &operator=(Runner&&) = default;
};

// ----------------------------------------------

}
