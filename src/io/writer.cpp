#include <cassert>
#include <cstdio>
#include <iostream>
#include <vector>

#include "../utils.h"
#include "writer.h"


namespace wolf {

// ----------------------------------------------

void Writer::writeln(const std::vector<std::string>& data) {
    for (const auto& d : data) {
        this->writeln(d.c_str());
    }
}

// ----------------------------------------------

void ConsoleWriter::writeln(const std::string& data) {
    std::cout << data << "\n";
}

// ----------------------------------------------

FileWriter::FileWriter(std::string filename) : filename(filename) {
    file_stream.open(filename);
    assert(file_stream.is_open());
}

void FileWriter::writeln(const std::string& data) {
    file_stream << data << "\n";
}

FileWriter::~FileWriter() {
    file_stream.close();
    assert(!file_stream.is_open());
}

// ----------------------------------------------

}
