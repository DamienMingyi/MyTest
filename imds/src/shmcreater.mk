BIN:= shmcreater
INC = 
LIBS =
VPATH = $(shell find ../../src -type d)

SRCLIST = $(shell find ../../src/shmcreater -name *.cpp)
SRCLIST += $(shell find ../../src/util/shm -name *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(notdir $(SRCLIST)))

all: $(BIN)

$(OBJS): %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INC)

$(BIN):$(OBJS)
	$(CXX) -o $(BIN) $(OBJS) $(LIBS)
	mv $(BIN) ../../bin/$(BIN)