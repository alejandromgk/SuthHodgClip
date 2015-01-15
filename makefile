all:
	g++ main.cpp point.cpp edgetable.cpp -lm -lGLU -lglut -lGL -std=gnu++0x -o ejecutableP2 
clean:
	rm ejecutableP2
