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

    std::vector<std::string> read_lines() override;

private:
    std::string     filename_;
    std::ifstream   file_stream_;
};

// ----------------------------------------------

}

