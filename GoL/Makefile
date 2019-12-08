default:
	make timer
	make cuda
	make seq

timer: timing.h gameOfLife.h gameOfLifeSequential.h gameOfLifeSequential.cpp gameOfLifeCuda.h gameOfLifeCuda.cu timer.cpp
	g++ -std=c++1y -m64 gameOfLifeSequential.cpp -O3 -Wall -g -c -o objs/gameOfLifeSequential.o
	g++ -std=c++1y -m64 timer.cpp -O3 -Wall -g -c -o objs/timer.o
	nvcc gameOfLifeCuda.cu -O3 --std=c++11 -m64 --gpu-architecture compute_35 -c -o objs/cudaRenderer.o
	g++ -std=c++1y -m64 -O3 -Wall -g -o timer objs/timer.o objs/cudaRenderer.o objs/gameOfLifeSequential.o -L/usr/local/depot/cuda-8.0/lib64/ -lcudart -lGL -lglut -lcudart

cuda: timing.h gameOfLife.h gameOfLifeSequential.h gameOfLifeSequential.cpp gameOfLifeCuda.h gameOfLifeCuda.cu cudamain.cpp
	g++ -std=c++1y -m64 gameOfLifeSequential.cpp -O3 -Wall -g -c -o objs/gameOfLifeSequential.o
	g++ -std=c++1y -m64 cudamain.cpp -O3 -Wall -g -c -o objs/cudamain.o
	nvcc gameOfLifeCuda.cu -O3 --std=c++11 -m64 --gpu-architecture compute_35 -c -o objs/cudaRenderer.o
	g++ -std=c++1y -m64 -O3 -Wall -g -o cudaGame objs/cudamain.o objs/cudaRenderer.o objs/gameOfLifeSequential.o -L/usr/local/depot/cuda-8.0/lib64/ -lcudart -lGL -lglut -lcudart

seq : gameOfLife.h gameOfLifeSequential.h gameOfLifeSequential.cpp seqmain.cpp
	g++ -std=c++1y -m64 -O3 -Wall -g -o seqGame gameOfLife.h gameOfLifeSequential.h gameOfLifeSequential.cpp seqmain.cpp
