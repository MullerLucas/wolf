#include <cstddef>
#include <memory>

#include "utils.h"
#include "io/writer.h"
#include "io/reader.h"
#include "testing.h"

#include "utils.cpp"
#include "io/writer.cpp"
#include "io/reader.cpp"
#include "testing.cpp"



int main(int argc, char** argv) {
    using namespace wolf;

    // HACK(lm): for testing purposes only
    {
        // std::unique_ptr<Reader> reader = std::make_unique<ConsoleReader>();
        std::unique_ptr<Reader> reader = std::make_unique<FileReader>("data/test_4_1.txt");
        auto data = reader->readln_all();

        std::unique_ptr<Writer> writer = std::make_unique<ConsoleWriter>();
        writer->writeln(data);
    }

    // HACK(lm): for testing purposes only
    // {
    //     const auto test_data = generate_test_data(4, true);
    //
    //     std::unique_ptr<Writer> writer = std::make_unique<ConsoleWriter>();
    //     // std::unique_ptr<Writer> writer = std::make_unique<FileWriter>("test.txt");
    //     writer->writeln(test_data);
    // }

    return 0;
}

