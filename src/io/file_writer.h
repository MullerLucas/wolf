#pragma once

#include "writer.h"

namespace wolf {

// ----------------------------------------------

class FileWriter : public Writer {
public:
    FileWriter(std::string file_name);
    ~FileWriter() override;

    FileWriter(const FileWriter&) = delete;
    FileWriter &operator=(const FileWriter&) = delete;
    FileWriter(FileWriter&&) = default;
    FileWriter &operator=(FileWriter&&) = default;

    Writer &operator<<(const std::string &message) override;
    Writer &operator<<(i64 number) override;
    Writer &operator<<(std::ostream &(*manipulator)(std::ostream&)) override;

private:
    std::string   filename_;
    std::ofstream file_stream_;
};

// ----------------------------------------------

}
