#include <algorithm>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "incremental_filter_runner.h"
#include "../filter/multi_trie_filter.h"
#include "../gui/filter_window.h"

namespace wolf {

// ----------------------------------------------

IncrementalFilterRunner::IncrementalFilterRunner(const Config& config)
    : Runner(config), window_("hell-app", 640, 730, 32.0f)
{
}

void IncrementalFilterRunner::run()
{
    Timer t;

    // read input
    const auto input = read_word_list();

    // create filter
    t.restart();
    auto filter  = std::make_unique<MultiTrieFilter>(config_.thread_count);
    auto session = filter->create_session();
    t.stop();
    log_info("Create filter: %i us\n", t.elapsed_us().count());

    // insert data
    t.restart();
    filter->insert_all(input);
    t.stop();
    log_info("Insert data: %i us\n", t.elapsed_us().count());

    state_.prev_word_count = input.size();
    state_.curr_word_count = input.size();

    // handle input changes
    const auto input_change_handler_ = [this, &t, &filter, &session] (const char* input) {
        log_info("Filter input changed: %s\n", input);

        // fliter word-list
        t.restart();
        {
            const auto input_len = strlen(input);
            if (input_len > session.depth) {
                filter->push_filter(session, &input[session.depth]);
            } else {
                filter->pop_filter(session, session.depth - input_len);
            }
            filter->collect(session);
        }
        t.stop();
        const auto filter_dur_us = t.elapsed_us().count();

        // collect result
        t.restart();
        {
            filter->collect(session);
        }
        t.stop();
        const auto collect_dur_us = t.elapsed_us().count();
        const auto total_dur_us = filter_dur_us + collect_dur_us;

        // update state
        {
            state_.prev_word_count = state_.curr_word_count;
            state_.curr_word_count = session.filtered_.size();

            state_.max_timing_us_ = *std::max_element(state_.timings_us_.begin(), state_.timings_us_.end());
            state_.timings_us_.erase(state_.timings_us_.begin());
            state_.timings_us_.push_back(total_dur_us);

            state_.words_.clear();
            for (const auto& f : session.filtered_) {
                state_.words_.push_back(f->c_str());
            }
        }

        log_info("Remaining-Words: %i\n", session.filtered_.size());
        log_info("Iteration: %i us = %i us (filter) + %i us (collect)\n", total_dur_us, filter_dur_us, collect_dur_us);

        return true;
    };
    input_change_handler_("");

    // init window
    window_.set_state(&state_);
    window_.set_input_changed_handler(input_change_handler_);
    window_.set_print_clicked_handler([this] {
        should_close_ = true;
    });

    // show window
    {
        window_.create();

        while (!should_close_ && !window_.should_close()) {
            window_.draw_frame();
        }

        window_.destroy();
    }

    writer_->write_lines(session.filtered_);
}

// ----------------------------------------------

}
