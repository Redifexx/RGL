CXX := g++
CXXFLAGS := -g --std=c++17
INCLUDE_DIR := -I../include
LIB_DIR := -L../lib
LIBRARIES := -lglfw3dll -lopengl32 -lgdi32 -limm32 -lassimp.dll
SOURCE_FILES := $(wildcard ../src/*.cpp) $(wildcard ../src/*.c)
OBJECT_FILES := $(patsubst ../src/%.cpp, obj/%.o, $(SOURCE_FILES)) \
                $(patsubst ../src/%.c, obj/%.o, $(SOURCE_FILES))
EXECUTABLE := rgl.exe

.PHONY: all build run clean

all: build

build: $(EXECUTABLE)

# Rule to build the executable
$(EXECUTABLE): $(OBJECT_FILES)
	$(CXX) $(CXXFLAGS) $(OBJECT_FILES) $(LIB_DIR) $(LIBRARIES) -o $@

# Rule to build object files
obj/%.o: ../src/%.cpp
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) -c $< -o $@

obj/%.o: ../src/%.c
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) -c $< -o $@

run: build
	./$(EXECUTABLE)

clean:
	del /Q .\$(EXECUTABLE)
	del /Q .\obj\*.o
