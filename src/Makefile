cuda: gameOfCivilization.h gameOfCivilizationCuda.h gameOfCivilizationCuda.cu cudamain.cpp
	g++ -std=c++1y -m64 gameOfCivilizationSequential.cpp -O3 -Wall -g -c -o objs/gameOfCivilizationSequential.o
	g++ -std=c++1y -m64 cudamain.cpp -O3 -Wall -g -c -o objs/cudamain.o
	nvcc gameOfCivilizationCuda.cu -O3 --std=c++11 -m64 --gpu-architecture compute_35 -c -o objs/cudaRenderer.o
	g++ -std=c++1y -m64 -O3 -Wall -g -o cudaGame objs/cudamain.o objs/cudaRenderer.o objs/gameOfCivilizationSequential.o -L/usr/local/depot/cuda-8.0/lib64/ -lcudart -lGL -lglut -lcudart

seq : gameOfCivilization.h gameOfCivilizationSequential.h gameOfCivilizationSequential.cpp main.cpp
	g++ -std=c++1y -m64 -O3 -Wall -g -o seqGame gameOfCivilization.h gameOfCivilizationSequential.h gameOfCivilizationSequential.cpp main.cpp
