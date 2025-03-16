CXXFLAGS=-ggdb -std=c++20 -Wall -Wextra -Werror -Wfatal-errors -pedantic
FILES=src/ladder.cpp src/ladder_main.cpp

main: src/ladder_main.cpp
	g++ $(CXXFLAGS) $(FILES) -o main
	./main
clean:
	/bin/rm main