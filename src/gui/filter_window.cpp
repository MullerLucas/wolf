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

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#include "../utils.h"

namespace wolf {

// ----------------------------------------------

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


FilterWindow::FilterWindow(const char *title, usize width, usize height,
                           float font_size)
    : title_(title), width_(width), height_(height), font_size_(font_size)
{ }

inline bool FilterWindow::should_close() const
{
    return glfwWindowShouldClose(window_);
}

void FilterWindow::create()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        std::runtime_error("Failed to initialize GLFW!");

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    window_ = glfwCreateWindow(width_, height_, title_, nullptr, nullptr);
    if (window_ == nullptr)
        std::runtime_error("Failed to create GLFW window!");

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1); // Enable vsync
    glfwSetWindowSizeLimits(window_, width_, height_, width_, height_);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup fonts
    {
        constexpr float FONT_SIZE_BIG   = 32.0f * 1.2f;
        constexpr float FONT_SIZE_SMALL = 32.0f * 0.8f;

        io.Fonts->AddFontDefault();
        font_norm_reg_ = io.Fonts->AddFontFromFileTTF("assets/fonts/open_sans/static/OpenSans-Regular.ttf", font_size_);
        IM_ASSERT(font_norm_reg_ != nullptr);
        font_norm_bold_ = io.Fonts->AddFontFromFileTTF("assets/fonts/open_sans/static/OpenSans-Bold.ttf", font_size_);
        IM_ASSERT(font_norm_bold_ != nullptr);
        font_big_bold_ = io.Fonts->AddFontFromFileTTF("assets/fonts/open_sans/static/OpenSans-Bold.ttf", FONT_SIZE_BIG);
        IM_ASSERT(font_big_bold_ != nullptr);
        font_small_reg_ = io.Fonts->AddFontFromFileTTF("assets/fonts/open_sans/static/OpenSans-Regular.ttf", FONT_SIZE_SMALL);
        IM_ASSERT(font_small_reg_ != nullptr);
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

void FilterWindow::draw_frame()
{
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::PushFont(font_norm_reg_);

    // ImGui::ShowDemoWindow(&show_demo_window);
    draw_filter_window();

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window_, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(COLOR_CLEAR_.x * COLOR_CLEAR_.w,
                 COLOR_CLEAR_.y * COLOR_CLEAR_.w,
                 COLOR_CLEAR_.z * COLOR_CLEAR_.w,
                 COLOR_CLEAR_.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window_);

    ++curr_frame_;
}

void FilterWindow::draw_filter_window()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(width_, height_));

    ImGui::Begin("Filter-Window", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    // window
    {
        // input section
        {
            // input label
            {
                ImGui::PushFont(font_big_bold_);
                ImGui::Text("Input");
                ImGui::PopFont();
            }

            // input text
            {
                ImGui::PushFont(font_small_reg_);
                ImGui::Text("Enter a word to filter the list with.");
                ImGui::PopFont();
            }

            ImGui::Spacing();

            // input field
            {
                constexpr auto FLAGS = ImGuiInputTextFlags_NoUndoRedo |
                    ImGuiInputTextFlags_CallbackEdit;

                auto cb = [](ImGuiInputTextCallbackData* data) -> int {
                    FilterWindow* self = (FilterWindow*)data->UserData;

                    if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit) {
                        if (self->input_changed_handler_ != nullptr) {
                            (void)self->input_changed_handler_(data->Buf);
                        }
                    }
                    return 0;
                };

                if (curr_frame_ == 0) {
                    ImGui::SetKeyboardFocusHere(0);
                }
                ImGui::SetNextItemWidth(-1);
                ImGui::InputText("Input", state_->input_buff,
                                 IM_ARRAYSIZE(state_->input_buff), FLAGS, cb,
                                 this);
            }
        }

        // separator
        {
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
        }

        // preview section
        {
            // preview label
            {
                ImGui::PushFont(font_big_bold_);
                ImGui::Text("Preview");
                ImGui::PopFont();
            }

            // stats text
            {
                ImGui::PushFont(font_small_reg_);
                ImGui::Text("Displaying");
                ImGui::SameLine();
                ImGui::TextColored(COLOR_HI_0_, "%zu", state_->view_word_count);
                ImGui::SameLine();
                ImGui::Text("out of");
                ImGui::SameLine();
                ImGui::TextColored(COLOR_HI_0_, "%zu", state_->curr_word_count);
                ImGui::SameLine(); ImGui::Text("words.");
                ImGui::PopFont();
            }

            ImGui::Spacing();

            // preview list
            {
                constexpr usize LIST_HEIGHT = 10;
                if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, LIST_HEIGHT * ImGui::GetTextLineHeightWithSpacing()))) {
                    for (const auto& word : state_->words)
                        (void)ImGui::Selectable(word, false);
                    ImGui::EndListBox();
                }
            }

            ImGui::Spacing();

            // print button
            {
                ImGui::SetNextItemWidth(-1);
                if (ImGui::Button(" Print & Exit ")) {
                    if (print_clicked_handler != nullptr)
                        print_clicked_handler();
                }
            }
        }

        // separator
        {
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
        }

        // stats section
        {
            // stats label
            {
                ImGui::PushFont(font_big_bold_);
                ImGui::Text("Stats");
                ImGui::PopFont();
            }

            // stats text
            {
                ImGui::PushFont(font_small_reg_);
                ImGui::Text("From");
                ImGui::SameLine();
                ImGui::TextColored(COLOR_HI_0_, "%lu", state_->prev_word_count);
                ImGui::SameLine();
                ImGui::Text("words, to");
                ImGui::SameLine();
                ImGui::TextColored(COLOR_HI_0_, "%lu", state_->curr_word_count);
                ImGui::SameLine();
                ImGui::Text("words, in");
                ImGui::SameLine();
                ImGui::TextColored(COLOR_HI_0_, "%ld", (i64)state_->timings_us.back());
                ImGui::SameLine();
                ImGui::Text("us!");
                ImGui::PopFont();
            }

            ImGui::Spacing();

            // chart
            {
                ImGui::SetNextItemWidth(-1);
                ImGui::PlotHistogram("Histogram", state_->timings_us.data(),
                                     state_->timings_us.size(), 0, NULL, 0.0f,
                                     state_->max_timing_us * 1.1f,
                                     ImVec2(0, 80.0f));
            }
        }
    }

    ImGui::PopFont();
    ImGui::End();

}

void FilterWindow::set_input_changed_handler(InputChangedHandler handler)
{
    input_changed_handler_ = handler;
}

void FilterWindow::set_print_clicked_handler(PrintClickedHandler handler)
{
    print_clicked_handler = handler;
}

void FilterWindow::set_state(FilterWindowState *state)
{
    state_ = state;
}

// ----------------------------------------------

}
