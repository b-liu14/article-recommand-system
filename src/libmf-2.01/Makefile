CXX = g++
CXXFLAGS = -Wall -O1 -g -pthread -std=c++0x -march=native
SHVER = 2

# run `make clean all' if you change the following flags.

# comment the following flag if you want to disable SSE or enable AVX
DFLAG = -DUSESSE

# uncomment the following flags if you want to use AVX
#DFLAG = -DUSEAVX
#CXXFLAGS += -mavx

# uncomment the following flags if you do not want to use OpenMP
DFLAG += -DUSEOMP
CXXFLAGS += -fopenmp

all: mf-train mf-predict mf-recommend mf-test preprocess

lib: 
	$(CXX) -shared -Wl,-soname,libmf.so.$(SHVER) -o libmf.so.$(SHVER) mf.o 

mf-train: mf-train.cpp mf.o
	$(CXX) $(CXXFLAGS) $(DFLAG) -o $@ $^

mf-predict: mf-predict.cpp mf.o
	$(CXX) $(CXXFLAGS) $(DFLAG) -o $@ $^

mf-recommend: mf-recommend.cpp mf.o
	$(CXX) $(CXXFLAGS) $(DFLAG) -o $@ $^

mf-test: mf-test.cpp mf.o
	$(CXX) $(CXXFLAGS) $(DFLAG) -o $@ $^

preprocess: preprocess.cpp mf.o
	$(CXX) $(CXXFLAGS) $(DFLAG) -o $@ $^

mf.o: mf.cpp mf.h
	$(CXX) $(CXXFLAGS) $(DFLAG) -c -fPIC -o $@ $<

clean:
	rm -f mf-train mf-predict mf-recommend mf-test mf.o libmf.so.$(SHVER)
