CXXFLAGS=-ggdb -std=c++20 -Wall -Wextra -Werror -Wfatal-errors -pedantic
FILES=src/dijkstras.cpp src/dijkstras_main.cpp src/ladder.cpp 

main: src/dijkstras_main.cpp
	g++ $(CXXFLAGS) $(FILES) -o main
	./main
clean:
	/bin/rm main