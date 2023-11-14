#pragma once

#include <iostream>
#include <fstream>
#include <vector>


namespace wolf {

// ----------------------------------------------

class Reader {
public:
    virtual std::vector<std::string> readln_all() = 0;
    virtual ~Reader() = default;
};

// ----------------------------------------------

class ConsoleReader: public Reader {
public:
    std::vector<std::string> readln_all();
};

// ----------------------------------------------

class FileReader: public Reader {
public:
    FileReader(std::string file_name);
    ~FileReader() override;

    std::vector<std::string> readln_all() override;

private:
    std::string filename_;
    std::ifstream file_stream_;
};

// ----------------------------------------------

}

