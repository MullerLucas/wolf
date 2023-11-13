#include <cstddef>
#include <memory>

#include "utils.h"
#include "testing.h"
#include "io/writer.h"
#include "io/reader.h"
#include "filter/simple_filter.h"

#include "utils.cpp"
#include "testing.cpp"
#include "io/writer.cpp"
#include "io/reader.cpp"
#include "filter/simple_filter.cpp"



int main() {
    using namespace wolf;

    // HACK(lm): for testing purposes only
    {
        std::unique_ptr<Reader> reader = std::make_unique<FileReader>("data/input_w4_1.txt");
        auto input = reader->readln_all();

        SimpleFilter filter;
        const auto output = filter.filter("AAA", input, 20);

        std::unique_ptr<Writer> writer = std::make_unique<ConsoleWriter>();
        writer->writeln_all(output);
    }

    // HACK(lm): for testing purposes only
    // {
    //     const auto test_data = generate_test_data(5, true);
    //
    //     std::unique_ptr<Writer> writer = std::make_unique<ConsoleWriter>();
    //     writer->writeln_all(test_data);
    // }

    return 0;
}
