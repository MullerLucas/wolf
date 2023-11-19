#pragma once

#include "imgui_impl_glfw.h"
#include "../utils.h"

namespace wolf {

// ----------------------------------------------

class FilterWindow {
public:
    FilterWindow(const char* title, usize width, usize height, float font_size);

    bool should_close() const;
    void create();
    void draw();
    void destroy();

private:
    static constexpr usize  INPUT_BUFF_SIZE_    = 64;
    static constexpr ImVec4 COLOR_HI_0_         = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
    static constexpr ImVec4 COLOR_CLEAR_        = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    const char* title_ = "hell-app";
    usize       width_;
    usize       height_;
    float       font_size_;

    GLFWwindow* window_;
    ImFont*     font_norm_reg_;
    ImFont*     font_norm_bold_;
    ImFont*     font_big_bold_;

    bool        show_demo_window = false;

    void draw_filter_window();
};

// ----------------------------------------------

}
