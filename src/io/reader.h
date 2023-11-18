#pragma once

#include <iostream>
#include <fstream>
#include <vector>


namespace wolf {

// ----------------------------------------------

class Reader {
public:
    virtual ~Reader() = default;
    virtual std::vector<std::string> read_lines() = 0;
};

// ----------------------------------------------

class ConsoleReader: public Reader {
public:
    std::vector<std::string> read_lines();
};

// ----------------------------------------------

class FileReader: public Reader {
public:
    FileReader(std::string file_name);
    ~FileReader() override;

    std::vector<std::string> read_lines() override;

private:
    std::string filename_;
    std::ifstream file_stream_;
};

// ----------------------------------------------

}

