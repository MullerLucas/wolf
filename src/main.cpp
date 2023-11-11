#include <cstddef>
#include <memory>

#include "utils.h"
#include "writer.h"

#include "utils.cpp"
#include "writer.cpp"



int main(int argc, char** argv) {
    using namespace wolf;

    {
        std::unique_ptr<Writer> writer = std::make_unique<ConsoleWriter>();
        // std::unique_ptr<Writer> writer = std::make_unique<FileWriter>("test.txt");
        writer->write("Hello, wolf!\n");
    }

    return 0;
}

