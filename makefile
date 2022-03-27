CXX=g++
CXXFLAGS=-Wall -g -pthread

TARGETS=play test_playermontecarlo

all: $(TARGETS)

play: play.o player.o playerhuman.o playerminimax.o playermontecarlo.o game.o board.o
	$(CXX) $(CXXFLAGS) -o play play.o player.o playerhuman.o playerminimax.o playermontecarlo.o game.o board.o

test_playermontecarlo: test_playermontecarlo.o playermontecarlo.o player.o game.o board.o
	$(CXX) $(CXXFLAGS) -o test_playermontecarlo test_playermontecarlo.o playermontecarlo.o player.o game.o board.o

play.o: play.cpp play.h player.h playerhuman.h playerminimax.h playermontecarlo.h game.h
	$(CXX) $(CXXFLAGS) -c play.cpp player.cpp playerhuman.cpp playerminimax.cpp playermontecarlo.cpp game.cpp

test_playermontecarlo.o: test_playermontecarlo.cpp playermontecarlo.h player.h game.h board.h
	$(CXX) $(CXXFLAGS) -c test_playermontecarlo.cpp playermontecarlo.cpp player.cpp game.cpp board.cpp

playermontecarlo.o: playermontecarlo.cpp playermontecarlo.h player.h game.h
	$(CXX) $(CXXFLAGS) -c playermontecarlo.cpp player.cpp game.cpp

playerminimax.o: playerminimax.cpp playerminimax.h player.h
	$(CXX) $(CXXFLAGS) -c playerminimax.cpp player.cpp

playerhuman.o: playerhuman.cpp playerhuman.h player.h
	$(CXX) $(CXXFLAGS) -c playerhuman.cpp player.cpp

player.o: player.cpp player.h game.h
	$(CXX) $(CXXFLAGS) -c player.cpp game.cpp

game.o: game.cpp game.h board.h
	$(CXX) $(CXXFLAGS) -c game.cpp board.cpp

board.o: board.cpp board.h
	$(CXX) $(CXXFLAGS) -c board.cpp

clean:
	rm -r $(TARGETS) *.o *.exe
