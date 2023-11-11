#pragma once

#include <iostream>
#include <fstream>


namespace wolf {

// ----------------------------------------------

class Writer {
public:
    virtual void write(const std::string& data) = 0;
    virtual ~Writer() = default;
};

// ----------------------------------------------

class ConsoleWriter : public Writer {
public:
    void write(const std::string& data) override;
};

// ----------------------------------------------

class FileWriter : public Writer {
public:
    FileWriter(const std::string& filename);
    void write(const std::string& data) override;
    ~FileWriter() override;

private:
    std::string filename;
    std::ofstream file_stream;
};

// ----------------------------------------------

}

