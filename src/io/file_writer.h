#pragma once

#include "writer.h"

namespace wolf {

// ----------------------------------------------

class FileWriter : public Writer {
public:
    FileWriter(std::string file_name);
    ~FileWriter() override;

    Writer &operator<<(const std::string &message) override;
    Writer &operator<<(i64 number) override;
    Writer &operator<<(std::ostream &(*manipulator)(std::ostream&)) override;

private:
    std::string   filename_;
    std::ofstream file_stream_;
};

// ----------------------------------------------

}
