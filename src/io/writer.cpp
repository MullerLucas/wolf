#include <cassert>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <vector>

#include "../utils.h"
#include "writer.h"

namespace wolf {

// ----------------------------------------------

void Writer::write_lines(const std::vector<std::string>& data) {
    for (const auto& d : data) {
        *this << d << "\n";
    }
}

void Writer::write_lines(const std::vector<const std::string*>& data) {
    for (const auto& d : data) {
        *this << *d << "\n";
    }
}

Writer& Writer::set_width(usize width) {
    width_ = width;
    return *this;
}

// ----------------------------------------------

Writer& ConsoleWriter::operator<<(std::ostream& (*manipulator)(std::ostream&)) {
    std::cout << manipulator;
    return *this;
}

Writer& ConsoleWriter::operator<<(const std::string& message) {
    std::cout << std::setw(width_) << message;
    return *this;
}

Writer& ConsoleWriter::operator<<(i64 number)  {
    std::cout << std::setw(width_) << number;
    return *this;
}

// ----------------------------------------------

FileWriter::FileWriter(std::string filename) : filename_(filename) {
    file_stream_.open(filename);
    assert(file_stream_.is_open());
}

Writer& FileWriter::operator<<(std::ostream& (*manipulator)(std::ostream&)) {
    file_stream_ << manipulator;
    return *this;
}

Writer& FileWriter::operator<<(const std::string& message) {
    file_stream_ << std::setw(width_) << message;
    return *this;
}

Writer& FileWriter::operator<<(i64 number)  {
    file_stream_ << std::setw(width_) << number;
    return *this;
}

FileWriter::~FileWriter() {
    file_stream_.close();
    assert(!file_stream_.is_open());
}

// ----------------------------------------------

}
