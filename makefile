CXX=g++
CXXFLAGS=-Wall -g

TARGETS=play

play: play.cpp
	$(CXX) $(CXXFLAGS) -o play play.cpp game.cpp player.cpp board.cpp

clean:
	rm -r $(TARGETS)
