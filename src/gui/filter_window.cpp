// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "filter_window.h"


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdexcept>
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#include "../utils.h"

namespace wolf {

// ----------------------------------------------

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


FilterWindow::FilterWindow(const char* title, usize width, usize height, float font_size)
    : title_(title), width_(width), height_(height), font_size_(font_size)
{ }

inline bool FilterWindow::should_close() const {
    return glfwWindowShouldClose(window_);
}

// Main code
void FilterWindow::create()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::runtime_error("Failed to initialize GLFW!");
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    window_ = glfwCreateWindow(width_, height_, title_, nullptr, nullptr);
    if (window_ == nullptr) {
        std::runtime_error("Failed to create GLFW window!");
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup fonts
    {
        constexpr float FONT_SIZE_BIG = 32.0f * 1.25f;

        io.Fonts->AddFontDefault();
        font_norm_reg_ = io.Fonts->AddFontFromFileTTF("assets/fonts/open_sans/static/OpenSans-Regular.ttf", font_size_);
        IM_ASSERT(font_norm_reg_ != nullptr);
        font_norm_bold_ = io.Fonts->AddFontFromFileTTF("assets/fonts/open_sans/static/OpenSans-Bold.ttf", font_size_);
        IM_ASSERT(font_norm_bold_ != nullptr);
        font_big_bold_ = io.Fonts->AddFontFromFileTTF("assets/fonts/open_sans/static/OpenSans-Bold.ttf", FONT_SIZE_BIG);
        IM_ASSERT(font_big_bold_ != nullptr);
    }
}

void FilterWindow::destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window_);
    glfwTerminate();
}

void FilterWindow::draw()
{
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::PushFont(font_norm_reg_);

    // TODO (lm): remove
    // demo window
    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    draw_filter_window();

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window_, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(COLOR_CLEAR_.x * COLOR_CLEAR_.w, COLOR_CLEAR_.y * COLOR_CLEAR_.w, COLOR_CLEAR_.z * COLOR_CLEAR_.w, COLOR_CLEAR_.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window_);

}

void FilterWindow::draw_filter_window()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(width_, height_)); // Set the window size as per your OpenGL window

    ImGui::Begin("Filter-Window", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
    {
        // header
        {
            ImGui::PushFont(font_big_bold_);
            ImGui::Text("Word-List-Filter");
            ImGui::PopFont();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // filter input
        {
            static char buf1[64];
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("Input", buf1, IM_ARRAYSIZE(buf1), ImGuiInputTextFlags_NoUndoRedo);
        }

        // filter preview
        {
            const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
            static int item_current_idx = 0; // Here we store our selection data as an index.

            ImGui::PushFont(font_norm_bold_);
            ImGui::Text("Preview");
            ImGui::PopFont();

            constexpr usize LIST_HEIGHT = 10;
            if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, LIST_HEIGHT * ImGui::GetTextLineHeightWithSpacing())))
            {
                for (int n = 0; n < IM_ARRAYSIZE(items); n++)
                {
                    const bool is_selected = (item_current_idx == n);
                    if (ImGui::Selectable(items[n], is_selected))
                        item_current_idx = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }
        }

        ImGui::Spacing();

        // buttons
        {
            ImGui::SetNextItemWidth(-1);
            if (ImGui::Button("Print")) {
                log_info("Print");
            }
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // filter stats
        {
            // filter Info
            {
                ImGui::Text("Filtered");
                ImGui::SameLine(); ImGui::TextColored(COLOR_HI_0_, "123");
                ImGui::SameLine(); ImGui::Text("words, in");
                ImGui::SameLine(); ImGui::TextColored(COLOR_HI_0_, "123");
                ImGui::SameLine(); ImGui::Text("seconds!");
            }

            // charts
            {
                static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
                // ImGui::PlotLines("Frame Times", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f, ImVec2(0, 80.0f));
                ImGui::PlotHistogram("Histogram", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f, ImVec2(0, 80.0f));
            }
        }

        // demo
        {
            ImGui::PushFont(font_norm_bold_);
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::PopFont();
        }

    }

    ImGui::PopFont();
    ImGui::End();

}

// ----------------------------------------------

}
