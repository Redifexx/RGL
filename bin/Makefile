lCXX := g++
CXXFLAGS := -g --std=c++17
INCLUDE_DIR := -I../include
LIB_DIR := -L../lib
LIBRARIES := -lglfw3dll -lopengl32 -lgdi32 -limm32 -lassimp.dll
SOURCE_FILES := ../src/main.cpp ../src/*.c #../include/imgui/*.cpp
EXECUTABLE := rgl.exe

.PHONY: all build run clean

all: build run

build: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE_FILES)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) $(LIB_DIR) $^ $(LIBRARIES) -o $@

run: build
	./$(EXECUTABLE)

clean:
	del /Q .\$(EXECUTABLE)
