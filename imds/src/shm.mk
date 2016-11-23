SHM := libshm.so
#BIN := shm
INC =
LIBS= 

VPATH =$(shell find ../../src -type d)

SHMLIST=$(shell find ../../src/util/shm -name *.cpp)

OBJS=$(patsubst %.cpp,%.o,$(notdir $(SHMLIST)))


all:$(SHM)
	
$(OBJS):%.o:%.cpp
	$(CXX) -c -std=c++11 -fPIC $(CXXFLAGS) $< -o $@ $(INC)
#	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INC)

$(SHM):$(OBJS)
	$(CXX) -shared -fPIC  -o $(SHM) $(OBJS)
	mv $(SHM) ../../bin/$(SHM)

	