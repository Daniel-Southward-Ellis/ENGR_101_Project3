SFML = /usr/local/include/SFML
CFLAGS = -I ${SFML}/include -std=c++0x
LFLAGS = -L /usr/local/lib
LIBS = -lsfml-window  -lsfml-graphics -lsfml-system -lsfml-network 
server3: server3.o 
	g++ $(LFLAGS) -o server3 server3.o ${LIBS} 
server3.o: server3.cpp 
	g++  -c $(CFLAGS) server3.cpp 


