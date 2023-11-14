#include <cassert>
#include <cstdio>
#include <iostream>
#include <vector>

#include "../utils.h"
#include "writer.h"

namespace wolf {

// ----------------------------------------------

void Writer::writeln_all(const std::vector<std::string>& data) {
    for (const auto& d : data) {
        writeln(d);
    }
}

void Writer::writeln_all(const std::vector<const std::string*>& data) {
    for (const auto& d : data) {
        writeln(*d);
    }
}

// ----------------------------------------------

void ConsoleWriter::writeln(const std::string& data) {
    std::cout << data << "\n";
}

// ----------------------------------------------

FileWriter::FileWriter(std::string filename) : filename_(filename) {
    file_stream_.open(filename);
    assert(file_stream_.is_open());
}

void FileWriter::writeln(const std::string& data) {
    file_stream_ << data << "\n";
}

FileWriter::~FileWriter() {
    file_stream_.close();
    assert(!file_stream_.is_open());
}

// ----------------------------------------------

}
