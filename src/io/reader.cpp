#include "../utils.h"
#include "reader.h"
#include <cassert>
#include <string>
#include <vector>

namespace wolf {

// ----------------------------------------------

std::vector<std::string> ConsoleReader::readln_all() {
    std::string tmp;
    std::vector<std::string> input;

    while (std::cin >> tmp) {
        input.push_back(std::move(tmp));
    }

    return input;
}

// ----------------------------------------------

FileReader::FileReader(std::string file_name) : filename(file_name) {
    file_stream.open(filename);
    assert(file_stream.is_open());
}

FileReader::~FileReader() {
    file_stream.close();
    assert(!file_stream.is_open());
}

std::vector<std::string> FileReader::readln_all() {
    std::vector<std::string> input;

    std::string line;
    while (std::getline(file_stream, line)) {
        input.push_back(std::move(line));
    }

    return input;
}

}

