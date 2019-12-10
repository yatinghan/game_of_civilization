cuda: AreaDivisionParallel.cpp ../GoL/gameOfLife.h ../GoL/gameOfLifeCuda.h ../GoL/gameOfLifeCuda.cu gameOfCivilization.h gameOfCivilizationCuda.h gameOfCivilizationCuda.cpp cudamain.cpp
	nvcc ../GoL/gameOfLifeCuda.cu -O3 -g --std=c++11 -m64 --gpu-architecture compute_35 -c -o objs/cudaGoL.o
	#nvcc gameOfCivilizationCuda.cu -O3 -g -Xcompiler -fopenmp --std=c++11 -m64 --gpu-architecture compute_35 -c -o objs/cudaRenderer.o
	g++ -std=c++1y -m64 -fopenmp gameOfCivilizationCuda.cpp -O3 -g -c -o objs/cudaRenderer.o
	g++ -std=c++1y -m64 -fopenmp cudamain.cpp -O3 -g -c -o objs/cudamain.o
	g++ -std=c++1y -m64 -O3 -g -fopenmp -o cudaGame objs/cudaGoL.o objs/cudamain.o objs/cudaRenderer.o -L/usr/local/depot/cuda-8.0/lib64/ -lcudart -lGL -lglut -lcudart

baseline : ../GoL/gameOfLife.h ../GoL/gameOfLifeSequential.h ../GoL/gameOfLifeSequential.cpp gameOfCivilization.h gameOfCivilizationBaseline.h gameOfCivilizationBaseline.cpp baselinemain.cpp
	g++ -std=c++1y -m64 -O3 -g -o baselineGame ../GoL/gameOfLife.h ../GoL/gameOfLifeSequential.h ../GoL/gameOfLifeSequential.cpp gameOfCivilization.h gameOfCivilizationBaseline.h gameOfCivilizationBaseline.cpp baselinemain.cpp
 