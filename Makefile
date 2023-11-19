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

SRC_DIR := src
IMGUI_DIR := lib/imgui
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

#CXX = g++
CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wformat -pthread
CXXFLAGS += -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends

ifeq ($(MODE), debug)
	CXXFLAGS += -Og -g
	TARGET_DIR := target/debug
endif

ifeq ($(MODE), release)
	CXXFLAGS += -O3
	TARGET_DIR := target/release
endif

EXE = $(TARGET_DIR)/wolf

SOURCES = $(SRC_DIR)/main.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

OBJS = $(addprefix $(TARGET_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

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

$(TARGET_DIR)/%.o:$(SRC_DIR)/%.cpp | $(TARGET_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TARGET_DIR)/%.o:$(IMGUI_DIR)/%.cpp | $(TARGET_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TARGET_DIR)/%.o:$(IMGUI_DIR)/backends/%.cpp | $(TARGET_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)

$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)
