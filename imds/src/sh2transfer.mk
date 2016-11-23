BIN:= sh2transfer
INC = 
LIBS = -L/home/level2/lib -lboost_thread -lboost_system -lboost_filesystem -ldl -lpthread -lQuickFAST -lFastCodecs
VPATH = $(shell find ../../src -type d)

SRCLIST = $(shell find ../../src/sh2/transfer -name *.cpp)
SRCLIST += $(shell find ../../src/sh2/quothandle -name *.cpp)
SRCLIST += $(shell find ../../src/public/fast -name *.cpp)
SRCLIST += $(shell find ../../src/public/handle -name *.cpp)
SRCLIST += $(shell find ../../src/public/process -name *.cpp)
SRCLIST += $(shell find ../../src/public/market -name *.cpp)
SRCLIST += $(shell find ../../src/public/messagereader -name *.cpp)
SRCLIST += $(shell find ../../src/public/stock2 -name *.cpp)
SRCLIST += $(shell find ../../src/public/program -name *.cpp)
SRCLIST += $(shell find ../../src/public/protocol -name *.cpp)
SRCLIST += $(shell find ../../src/public/protocolfactory -name *.cpp)
SRCLIST += $(shell find ../../src/public/stream -name *.cpp)
SRCLIST += $(shell find ../../src/util/shm -name *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(notdir $(SRCLIST)))

all:$(BIN)

$(OBJS):%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INC)
$(BIN):$(OBJS)
	$(CXX) -o $(BIN) $(OBJS) $(LIBS)
	mv $(BIN) ../../bin/$(BIN)