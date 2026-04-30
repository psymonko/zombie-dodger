CXX = g++
CXXFLAGS = -g -std=c++17
LDFLAGS = -lGLEW -lGL -lSOIL -lSDL2 -lSDL2_ttf

TARGET = build/main
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)


b: $(TARGET) 

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

r: b
	./$(TARGET)

clean:
	rm -f src/*.o $(TARGET)
