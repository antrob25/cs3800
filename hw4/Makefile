CXX = g++
#CXX = mpicxx
CXXFLAGS = -Wall -Werror -W -std=c++11
LIBRARIES = -pthread
.PHONY: default run
default: run

run:
	${CXX} ${CXXFLAGS} *.cpp ${LIBRARIES} -o program 
	
clean:
	rm -f *.o program outFile*
