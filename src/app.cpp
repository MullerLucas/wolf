#include "app.h"
#include "utils.h"

namespace wolf {

// ----------------------------------------------

App::App(
    std::unique_ptr<Reader> reader,
    std::unique_ptr<Writer> writer,
    std::unique_ptr<Filter> filter,
    std::string             pattern
) : reader_ (std::move(reader)),
    writer_ (std::move(writer)),
    filter_ (std::move(filter)),
    pattern_(std::move(pattern))
{ }

// ----------------------------------------------

}
