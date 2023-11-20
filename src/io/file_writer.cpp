#include "file_writer.h"

namespace wolf {

// ----------------------------------------------

FileWriter::FileWriter(std::string filename)
    : filename_(filename)
{
    file_stream_.open(filename);
    assert(file_stream_.is_open());
}

Writer &FileWriter::operator<<(std::ostream &(*manipulator)(std::ostream&))
{
    file_stream_ << manipulator;
    return *this;
}

Writer &FileWriter::operator<<(const std::string &message)
{
    file_stream_ << std::setw(width_) << message;
    return *this;
}

Writer &FileWriter::operator<<(i64 number)
{
    file_stream_ << std::setw(width_) << number;
    return *this;
}

FileWriter::~FileWriter() {
    file_stream_.close();
    assert(!file_stream_.is_open());
}

// ----------------------------------------------

}
