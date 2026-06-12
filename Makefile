CXX     = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS  = -lncurses
TARGET   = snake_game
SRCS     = main.cpp board.cpp snake.cpp food.cpp poison.cpp gate.cpp contents.cpp

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
