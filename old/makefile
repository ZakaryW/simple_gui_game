CC=g++ -std=c++11 -g
exe=asm4
objs=bats.o event.o game.o gold.o pit.o room.o wumpus.o cave.o ladder.o

$(exe): main.cpp $(objs)
	$(CC) main.cpp $(objs) -o $(exe)

game.o: game.cpp game.h constants.h
	$(CC) -c game.cpp

cave.o: cave.cpp cave.h
	$(CC) -c cave.cpp

ladder.o: ladder.cpp ladder.h
	$(CC) -c ladder.cpp

bats.o: bats.cpp bats.h
	$(CC) -c bats.cpp

event.o: event.cpp event.h
	$(CC) -c event.cpp

gold.o: gold.cpp gold.h
	$(CC) -c gold.cpp

pit.o: pit.cpp pit.h
	$(CC) -c pit.cpp

room.o: room.cpp room.h
	$(CC) -c room.cpp

wumpus.o: wumpus.cpp wumpus.h
	$(CC) -c wumpus.cpp

clean:
	rm -f *.o $(exe)
