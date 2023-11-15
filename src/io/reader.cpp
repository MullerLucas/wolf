#include "../utils.h"
#include "reader.h"
#include <cassert>
#include <string>
#include <vector>

namespace wolf {

// ----------------------------------------------

std::vector<std::string> ConsoleReader::read_lines() {
    std::string tmp;
    std::vector<std::string> input;

    while (std::cin >> tmp) {
        input.push_back(std::move(tmp));
    }

    return input;
}

// ----------------------------------------------

FileReader::FileReader(std::string file_name) : filename_(file_name) {
    file_stream_.open(filename_);
    assert(file_stream_.is_open());
}

FileReader::~FileReader() {
    file_stream_.close();
    assert(!file_stream_.is_open());
}

std::vector<std::string> FileReader::read_lines() {
    std::vector<std::string> input;
    std::string line;

    while (std::getline(file_stream_, line)) {
        input.push_back(std::move(line));
    }

    return input;
}

}

