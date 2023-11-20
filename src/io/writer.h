#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "../utils.h"


namespace wolf {

// ----------------------------------------------

class Writer {
public:
    Writer() = default;
    virtual ~Writer() = default;

    virtual Writer &operator<<(const std::string& message) = 0;
    virtual Writer &operator<<(i64 number) = 0;
    virtual Writer &operator<<(std::ostream &(*manipulator)(std::ostream&)) = 0;

    virtual void write_lines(const std::vector<std::string> &data);
    virtual void write_lines(const std::vector<const std::string*> &data);

    Writer &set_width(usize width);

protected:
    usize width_ = 0;

    Writer(const Writer&) = default;
    Writer &operator=(const Writer&) = default;
    Writer(Writer&&) = default;
    Writer &operator=(Writer&&) = default;
};

// ----------------------------------------------

}

