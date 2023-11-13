#include <cstddef>
#include <memory>

#include "utils.h"
#include "writer.h"
#include "testing.h"

#include "utils.cpp"
#include "writer.cpp"
#include "testing.cpp"



int main(int argc, char** argv) {
    using namespace wolf;

    // HACK(lm): for testing purposes only
    {
        const auto test_data = generate_test_data(4, true);

        std::unique_ptr<Writer> writer = std::make_unique<ConsoleWriter>();
        // std::unique_ptr<Writer> writer = std::make_unique<FileWriter>("test.txt");
        writer->writeln(test_data);
    }

    return 0;
}

