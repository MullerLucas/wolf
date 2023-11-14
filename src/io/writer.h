#pragma once

#include <iostream>
#include <fstream>
#include <vector>


namespace wolf {

// ----------------------------------------------

class Writer {
public:
    virtual void writeln(const std::string& data) = 0;
    virtual void writeln_all(const std::vector<std::string>& data);
    virtual void writeln_all(const std::vector<const std::string*>& data);
    virtual ~Writer() = default;
};

// ----------------------------------------------

class ConsoleWriter : public Writer {
public:
    void writeln(const std::string& data) override;
};

// ----------------------------------------------

class FileWriter : public Writer {
public:
    FileWriter(std::string file_name);
    void writeln(const std::string& data) override;
    ~FileWriter() override;

private:
    std::string filename;
    std::ofstream file_stream;
};

// ----------------------------------------------

}
