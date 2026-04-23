SRCS = src/*.cpp
LIBS = -lGLEW -lGL -lSOIL -lSDL2 -lSDL2_ttf

b:
	g++ -g -o build/main $(SRCS) $(LIBS)

r: b
	./build/main
