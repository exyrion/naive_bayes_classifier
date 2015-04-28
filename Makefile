classify: classify.o
	g++ classify.o -o classify
classify.o: classify.cpp
	g++ classify.cpp -c