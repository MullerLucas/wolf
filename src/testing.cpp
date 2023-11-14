#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

#include "utils.h"
#include "testing.h"

namespace wolf {

std::vector<std::string> generate_test_data(usize width, b8 shuffle) {
    assert(width <= 26);

    u32 total_combinations = static_cast<int>(std::pow(26, width));

    std::vector<std::string> test_data;
    test_data.reserve(total_combinations);

    log_info("Generating %d combinations for len %i\n", total_combinations, width);

    for (u32 i = 0; i < total_combinations; ++i) {
        std::string combination(width, 'A');
        int temp = i;

        for (usize j = 0; j < width; j++) {
            combination[width - 1 - j] = static_cast<char>('A' + temp % 26);
            temp /= 26;
        }

        test_data.push_back(combination);
    }

    if (shuffle) {
        // Seed for the random number generator
        std::random_device rd;
        // Use the Mersenne Twister engine for randomness
        // https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine
        std::mt19937 g(rd());

        // Each possible permutation of those elements has equal probability of appearance.
        std::shuffle(test_data.begin(), test_data.end(), g);
    }

    return test_data;
}

}
