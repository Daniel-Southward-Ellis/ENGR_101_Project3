ifeq ($(OS),Windows_NT)
INCLUDE = -I C:\\SFML\include
LIBS = -L C:\\SFML\lib
robot.exe: robot.o
	g++ $(LIBS) -o robot robot.o -lsfml-window  -lsfml-graphics -lsfml-system -lsfml-network
robot.o: robot.cpp
	g++  -c $(INCLUDE) robot.cpp
else
SHELL_NAME := $(shell uname -s)
ifeq ($(SHELL_NAME),Linux)
SFML = ../../SFML-2.5.1
CFLAGS = -I ${SFML}/include 
LFLAGS = -L ${SFML}/lib -Wl,-R ${SFML}/lib 
LIBS = -lsfml-window  -lsfml-graphics -lsfml-system -lsfml-network 
robot: robot.o 
	g++ $(LFLAGS) -o robot robot.o ${LIBS} 
robot.o: robot.cpp 
	g++  -c $(CFLAGS) robot.cpp 
endif
ifeq ($(SHELL_NAME),Darwin)
SFML = /usr/local/lib/SFML
CFLAGS = -I ${SFML}/include
LFLAGS = -L /usr/local/lib
LIBS = -lsfml-window  -lsfml-graphics -lsfml-system -lsfml-network
robot: robot.o
	g++ $(LFLAGS) -o robot robot.o ${LIBS} 
robot.o: robot.cpp
	g++  -c $(CFLAGS) robot.cpp
endif
endif
