#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "../utils.h"


namespace wolf {

// ----------------------------------------------

class Writer {
public:
    virtual ~Writer() = default;

    virtual Writer& operator<<(const std::string& message) = 0;
    virtual Writer& operator<<(i64 number) = 0;
    virtual Writer& operator<<(std::ostream& (*manipulator)(std::ostream&)) = 0;

    virtual void write_lines(const std::vector<std::string>& data);
    virtual void write_lines(const std::vector<const std::string*>& data);

    Writer& set_width(usize width);

protected:
    usize width_ = 0;
};

// ----------------------------------------------

class ConsoleWriter : public Writer {
public:
    Writer& operator<<(const std::string& message) override;
    Writer& operator<<(i64 number) override;
    Writer& operator<<(std::ostream& (*manipulator)(std::ostream&)) override;
};

// ----------------------------------------------

class FileWriter : public Writer {
public:
    FileWriter(std::string file_name);
    ~FileWriter() override;

    Writer& operator<<(const std::string& message) override;
    Writer& operator<<(i64 number) override;
    Writer& operator<<(std::ostream& (*manipulator)(std::ostream&)) override;

private:
    std::string   filename_;
    std::ofstream file_stream_;
};

// ----------------------------------------------

}

