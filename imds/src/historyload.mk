BIN:= historyload
INC = 
LIBS =-L/home/level2/lib -lboost_thread -lboost_system -lboost_filesystem -ldl -lpthread -lQuickFAST -lFastCodecs
VPATH = $(shell find ../../src -type d)

SRCLIST = $(shell find ../../src/public/handle -name *.cpp)
SRCLIST += $(shell find ../../src/public/history -name *.cpp)
SRCLIST += $(shell find ../../src/public/program -name *.cpp)
SRCLIST += $(shell find ../../src/loader/history -name *.cpp)
SRCLIST += $(shell find ../../src/util/shm -name *.cpp)
SRCLIST += $(shell find ../../src/util/atof -name *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(notdir $(SRCLIST)))

all:$(BIN)

$(OBJS):%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INC)

$(BIN):$(OBJS)
	$(CXX) -o $(BIN) $(OBJS) $(LIBS)
	mv $(BIN) ../../bin/$(BIN)
