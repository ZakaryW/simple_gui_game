CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
 
# Directories
SRC_DIR = src
IMGUI_DIR = ImGui
 
# Source files
SRCS = driver.cpp GameGUI.cpp $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(IMGUI_DIR)/*.cpp)
 
# Output file
OUTPUT = driver
 
# Libraries
LIBS = -ldl -lGL -lglfw
 
# Targets
all: build
 
build:
	$(CXX) $(CXXFLAGS) $(SRCS) $(LIBS) -o $(OUTPUT)
 
clean:
	rm -f $(OUTPUT)