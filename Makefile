# --- 強制使用 Windows CMD (避免 MinGW sh.exe 導致 del 失效) ---
SHELL = cmd.exe

# --- 設定 SFML 路徑 ---
SFML_PATH = E:/Cenv/SFML-3.0.0

CXX = g++

# CXXFLAGS: 指定 include 位置
CXXFLAGS = -std=c++17 -Wall -Wextra -I"$(SFML_PATH)/include"

# LDFLAGS: 指定 lib 位置
LDFLAGS = -L"$(SFML_PATH)/lib" -lsfml-graphics -lsfml-window -lsfml-system -mwindows

# 目標檔案
TARGET = pacman_game.exe

# 來源檔案
SRCS = main.cpp Pathfinder.cpp PacMan.cpp Ghost.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 修正後的清理指令
clean:
	-del /F /Q *.o
	-del /F /Q $(TARGET)

.PHONY: all clean