#pragma once

#include "imgui_impl_glfw.h"

#include "../utils.h"

namespace wolf {

class FilterGui {
public:
    const usize WINDOW_WIDTH_ = 1280;
    const usize WINDOW_HEIGHT_ = 720;
    const char* WINDOW_TITLE_ = "hell-app";
    const usize INPUT_BUFFER_SIZE_ = 64;

    int setup();
    int render();
    int shutdown();

private:
    GLFWwindow* window_;
    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

};

}
