CXX = g++
CXXFLAGS = -g -Wall -MMD
OBJECTS = GraphTestHarness.o BCode.o Building.o Collection.o Graph.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = Graph

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

clean :
	rm -rf ${DEPENDS} ${OBJECTS} ${EXEC}

-include ${DEPENDS}