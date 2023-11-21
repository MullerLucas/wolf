#pragma once

#include "imgui_impl_glfw.h"
#include "../utils.h"

namespace wolf {

// ----------------------------------------------

struct FilterWindowState {
    static constexpr usize  INPUT_BUFF_SIZE_        = 64;
    static constexpr usize  MAX_PREVIEW_WORD_COUNT  = 500000;

    char                        input_buff[INPUT_BUFF_SIZE_];
    std::vector<const char*>    words;
    std::vector<float>          timings_us          = std::vector<float>(10, 0.0f);
    float                       max_timing_us       = 0.0f;
    usize                       curr_word_count     = 0;
    usize                       prev_word_count     = 0;
    usize                       view_word_count     = 0;
};

// ----------------------------------------------

class FilterWindow {
public:
    using InputChangedHandler = std::function<bool(const char*)>;
    using PrintClickedHandler = std::function<void()>;

    FilterWindow(const char *title, usize width, usize height, float font_size);

    FilterWindow(const FilterWindow&) = delete;
    FilterWindow &operator=(const FilterWindow&) = delete;
    FilterWindow(FilterWindow&&) = default;
    FilterWindow &operator=(FilterWindow&&) = default;

    bool should_close() const;
    void create();
    void draw_frame();
    void destroy();

    void set_input_changed_handler(std::function<bool(const char*)> handler);
    void set_print_clicked_handler(FilterWindow::PrintClickedHandler handler);
    void set_state(FilterWindowState *state);

private:
    static constexpr ImVec4 COLOR_HL_0_     = ImVec4( 0.9f,  0.7f,  0.0f,  1.0f);
    static constexpr ImVec4 COLOR_ClR_      = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    const char  *title_     = "WoLF";
    usize       width_;
    usize       height_;
    float       font_size_;
    usize       curr_frame_ = 0;

    GLFWwindow  *window_;
    ImFont      *font_norm_reg_;
    ImFont      *font_norm_bold_;
    ImFont      *font_big_bold_;
    ImFont      *font_small_reg_;

    FilterWindowState   *state_                 = nullptr;
    InputChangedHandler input_changed_handler_  = nullptr;
    PrintClickedHandler print_clicked_handler   = nullptr;

    void draw_filter_window();
    void draw_label() const;
};

// ----------------------------------------------

}
