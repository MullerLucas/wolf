#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "reader.h"

namespace wolf {

// ----------------------------------------------

class FileReader: public Reader {
public:
    FileReader(std::string file_name);
    ~FileReader() override;

    FileReader(const FileReader&) = delete;
    FileReader &operator=(const FileReader&) = delete;
    FileReader(FileReader&&) = default;
    FileReader &operator=(FileReader&&) = default;

    std::vector<std::string> read_lines() override;

private:
    std::string     filename_;
    std::ifstream   file_stream_;
};

// ----------------------------------------------

}

