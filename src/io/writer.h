#pragma once

#include <iostream>
#include <fstream>
#include <vector>


namespace wolf {

// ----------------------------------------------

class Writer {
public:
    virtual ~Writer() = default;

    virtual void writeln(const std::string& data) = 0;
    virtual void writeln_all(const std::vector<std::string>& data);
    virtual void writeln_all(const std::vector<const std::string*>& data);
};

// ----------------------------------------------

class ConsoleWriter : public Writer {
public:
    void writeln(const std::string& data) override;
};

// ----------------------------------------------

class FileWriter : public Writer {
public:
    ~FileWriter() override;

    FileWriter(std::string file_name);
    void writeln(const std::string& data) override;

private:
    std::string   filename_;
    std::ofstream file_stream_;
};

// ----------------------------------------------

}

