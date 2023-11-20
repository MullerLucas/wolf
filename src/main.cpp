#include "config.h"
#include "runner/runner.h"

using namespace wolf;

// ----------------------------------------------

int main(int argc, char **argv)
{
    // parse config
    Config config = config_from_args(argv + 1, argv + argc);
    if (config.is_verbose)
        log_config(config);

    // show help, if needed
    if (config.run_mode == RunMode::Help) {
        log_help();
        return 0;
    }

    // run runners
    {
        auto runner = Runner::create(&config);
        runner->run();
    }

    return 0;
}

// ----------------------------------------------
