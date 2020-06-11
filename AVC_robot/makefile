SFML = /usr/local/lib/SFML
CFLAGS = -I ${SFML}/include 
LFLAGS = -L /usr/local/lib
LIBS = -lsfml-window  -lsfml-graphics -lsfml-system -lsfml-network 
robot: robot.o 
	g++ $(LFLAGS) -o robot robot.o ${LIBS} 
robot.o: robot.cpp 
	g++  -c $(CFLAGS) robot.cpp 


