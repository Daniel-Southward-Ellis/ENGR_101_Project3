ifeq ($(OS),Windows_NT)
INCLUDE = -I C:\\SFML\include
LIBS = -L C:\\SFML\lib
robot.exe: robot.o
	g++ $(LIBS) -o robot robot.o -lsfml-window  -lsfml-graphics -lsfml-system -lsfml-network
robot.o: robot.cpp
	g++  -c $(INCLUDE) robot.cpp
else
#Our team is only using windows and mac so this is simply the mac makefile
SFML = /usr/local/lib/SFML
CFLAGS = -I ${SFML}/include
LFLAGS = -L /usr/local/lib
LIBS = -lsfml-window  -lsfml-graphics -lsfml-system -lsfml-network
robot: robot.o
	g++ $(LFLAGS) -o robot robot.o ${LIBS} 
robot.o: robot.cpp
	g++  -c $(CFLAGS) robot.cpp
endif