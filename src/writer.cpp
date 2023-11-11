#include <cstdio>
#include <iostream>

#include "utils.h"
#include "writer.h"



namespace wolf {

// ----------------------------------------------

void ConsoleWriter::write(const std::string& data) {
    std::cout << data;
}

// ----------------------------------------------

FileWriter::FileWriter(const std::string& filename) : filename(filename) {
    file_stream.open(filename);
}

void FileWriter::write(const std::string& data) {
    file_stream << data;
}

FileWriter::~FileWriter() {
    file_stream.close();
}

// ----------------------------------------------

}
