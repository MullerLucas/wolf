#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install libglfw-dev
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
#

MODE := release

TARGET_DIR := bin
OBJ_DIR := .cache/wolf
SRC_DIR := src
IMGUI_DIR := lib/imgui
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wformat -pthread
CXXFLAGS += -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends

ifeq ($(MODE), debug)
	CXXFLAGS += -Og -g
endif

ifeq ($(MODE), release)
	CXXFLAGS += -O3
endif

EXE = $(TARGET_DIR)/wolf

# wolf
# ----
SOURCES = $(SRC_DIR)/config.cpp $(SRC_DIR)/testing.cpp $(SRC_DIR)/utils.cpp $(SRC_DIR)/main.cpp $(SRC_DIR)/thread_pool.cpp $(SRC_DIR)/benchmark.cpp
SOURCES += $(SRC_DIR)/io/console_writer.cpp $(SRC_DIR)/io/writer.cpp $(SRC_DIR)/io/console_reader.cpp $(SRC_DIR)/io/file_reader.cpp $(SRC_DIR)/io/file_writer.cpp
SOURCES += $(SRC_DIR)/filter/simple_vector_filter.cpp $(SRC_DIR)/filter/multi_trie_filter.cpp
SOURCES += $(SRC_DIR)/collections/word_trie.cpp
SOURCES += $(SRC_DIR)/runner/incremental_filter_runner.cpp $(SRC_DIR)/runner/generator_runner.cpp $(SRC_DIR)/runner/runner.cpp $(SRC_DIR)/runner/oneshot_filter_runner.cpp $(SRC_DIR)/runner/benchmark_runner.cpp
SOURCES += $(SRC_DIR)/gui/filter_window.cpp

# imgui
# -----
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

LIBS =


##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	#LIBS += -lglfw3
	LIBS += -lglfw

	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32 -lpthread

	# 1. use gnu instead of msvc
	# 2. fix incompatibility between clang and gnu
	CXXFLAGS += -target x86_64-pc-windows-gnu -femulated-tls
	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

# imgui
# -----
$(OBJ_DIR)/%.o:$(IMGUI_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(IMGUI_DIR)/backends/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# wolf
# -----
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(SRC_DIR)/collections/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(SRC_DIR)/io/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(SRC_DIR)/filter/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(SRC_DIR)/runner/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(SRC_DIR)/gui/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS) | $(TARGET_DIR)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)

$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
