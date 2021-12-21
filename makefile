CXX=g++
CXXFLAGS=-Wall -g

TARGETS=play test_playermontecarlo

all: $(TARGETS)

play: play.cpp
	$(CXX) $(CXXFLAGS) -o play play.cpp game.cpp player.cpp playerhuman.cpp playerminimax.cpp playermontecarlo.cpp board.cpp

test_playermontecarlo: test_playermontecarlo.cpp
	$(CXX) $(CXXFLAGS) -o test_playermontecarlo test_playermontecarlo.cpp playermontecarlo.cpp player.cpp game.cpp board.cpp

clean:
	rm -r $(TARGETS) *.exe
