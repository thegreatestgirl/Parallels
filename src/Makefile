CC 	= g++ -std=c++17
GCOV_FLAG = -lgtest -pthread -lpthread
GRAPH_CPP_FILES = graph_parser.cpp s21_graph.cpp s21_graph_algorithms.cpp
TESTFILE = tests

COMMON_FILES = timer/timer.cpp\
				state_machine/state_machine.cpp\
				matrix.cpp\
				time_range/time_range.cpp

ANT_FILES = ant_algorithm/ant_algorithm.cpp interfaces/ant_interface/ant_interface.cpp\
				facades/ant/ant_facade.cpp\
				interfaces/ant_interface/main.cpp

GAUSS_FILES =  gauss_algorithm/gauss.cpp\
				facades/gauss/gauss_facade.cpp\
				interfaces/gauss/gauss_interface.cpp\
				interfaces/gauss/main.cpp

WINOGRAD_FILES = facades/winograd/winograd_facade.cpp\
				interfaces/winograd_interface/winograd_interface.cpp\
				winograd_algorithm/winograd.cpp\
				interfaces/winograd_interface/main.cpp

all: ant

clean:
	rm -rf *.a *.o *.out

gauss:
	$(CC) $(COMMON_FILES) $(GAUSS_FILES) -o gauss.out -lpthread
	./gauss.out

ant:
	$(CC) $(COMMON_FILES) $(ANT_FILES) -o ant.out -lpthread
	./ant.out

winograd:
	$(CC) $(COMMON_FILES) $(WINOGRAD_FILES) -o winograd.out -lpthread
	./winograd.out

