BIN := sh1list
INC =
LIBS=-lpthread 
VPATH =$(shell find ../../src -type d)

SRCLIST=$(shell find ../../src/sh1/list -name *.cpp)
SRCLIST+=$(shell find ../../src/util/shm -name *.cpp)
SRCLIST+=$(shell find ../../src/public/market -name *.cpp)
SRCLIST+=$(shell find ../../src/public/stock -name *.cpp)
SRCLIST+=$(shell find ../../src/public/program -name *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(notdir $(SRCLIST)))

all:$(BIN)
	
$(OBJS):%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INC)

$(BIN):$(OBJS)
	$(CXX) -o $(BIN) $(OBJS) $(LIBS)
	mv $(BIN) ../../bin/$(BIN)
