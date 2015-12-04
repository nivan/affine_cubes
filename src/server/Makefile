SRC_FILES=server.cpp ../affine_cube/boundingvolume.cpp  ../affine_cube/bvtree.cpp  ../affine_cube/Vector.cpp ../affine_cube/dotproductcomparison.cpp ../affine_cube/auxfunctions.cpp ../affine_cube/histogram2dbin.cpp ../miniball/miniballcomputation.cpp

all:
	g++ -std=c++11 -o run_server $(SRC_FILES) -I../affine_cube/ -I../miniball/ -I../../../crow/include/ -lboost_system -L/home/nivan/Qt/5.4/gcc_64/lib -lpthread 
