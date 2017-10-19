all: tinyxml build

tinyxml:
	g++ tinyxml2.cpp -c

classes:
	g++ Circle.cpp Window.cpp Player.cpp -c

build: tinyxml2.o Circle.o Window.o Player.o
	g++ -o trabalhocg main.cpp tinyxml2.o Circle.o Window.o Player.o -lglut -lGLU -lGL -lm

clean:
	rm tinyxml2.o Circle.o Window.o
	rm trabalhocg
