#include <cassert>
#include <cstddef>
#include <memory>

#include "utils.h"
#include "testing.h"
#include "io/writer.h"
#include "io/reader.h"
#include "filter/filter.h"
#include "filter/lock_free_list_filter.h"

#include "utils.cpp"
#include "testing.cpp"
#include "io/writer.cpp"
#include "io/reader.cpp"
#include "filter/lock_free_list_filter.cpp"


int main() {
    using namespace wolf;

    // HACK(lm): for testing purposes only
    {
        std::unique_ptr<Reader> reader = std::make_unique<FileReader>("data/input_w4_0.txt");
        auto input = reader->readln_all();
        assert(!input.empty());


        std::unique_ptr<Filter> filter = std::make_unique<LockFreeListFilter>(&input, 1);
        filter->filter("BB");
        filter->filter("C");
        filter->filter("A");

        std::unique_ptr<Writer> writer = std::make_unique<ConsoleWriter>();
        writer->writeln_all(filter->create_output());
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
