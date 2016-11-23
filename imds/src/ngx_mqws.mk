BIN := libmqws.so

#BIN := mqws
INC =
LIBS= -L ../../bin -lpthread -lshm

VPATH =$(shell find ../../src -type d)

UTILLIST=$(shell find ../../src/util/hash -name *.cpp)
UTILLIST+=$(shell find ../../src/util/jsoncpp -name *.cpp)
SRCLIST=$(shell find ../../src/service/ngx_service/ngx_mqws -name *.cpp)
SRCLIST+=$(shell find ../../src/public/market -name *.cpp)
SRCLIST+=$(shell find ../../src/public/stock -name *.cpp)
SRCLIST+=$(shell find ../../src/public/szstock -name *.cpp)
SRCLIST+=$(shell find ../../src/public/option -name *.cpp)
SRCLIST+=$(shell find ../../src/public/stock2 -name *.cpp)
#SRCLIST+=$(shell find ../../src/public/fixedincome -name *.cpp)
SRCLIST+=$(shell find ../../src/public/indexpress -name *.cpp)
SRCLIST+=$(shell find ../../src/public/hkpress -name *.cpp)
SRCLIST+=$(shell find ../../src/public/reply -name *.cpp)
SRCLIST+=$(shell find ../../src/public/program -name *.cpp)
SRCLIST+=$(shell find ../../src/service/stock -name *.cpp)
SRCLIST+=$(shell find ../../src/service/szstock -name *.cpp)
SRCLIST+=$(shell find ../../src/service/option -name *.cpp)
SRCLIST+=$(shell find ../../src/service/stock2 -name *.cpp)
#SRCLIST+=$(shell find ../../src/service/fixincome -name *.cpp)
SRCLIST+=$(shell find ../../src/service/indexpress -name *.cpp)
SRCLIST+=$(shell find ../../src/service/hkpress -name *.cpp)
UTILS=$(patsubst %.cpp,%.o,$(notdir $(UTILLIST)))
OBJS=$(patsubst %.cpp,%.o,$(notdir $(SRCLIST)))

all: $(BIN)
	
$(UTILS):%.o:%.cpp
	$(CXX) -c -std=c++11 -fPIC $(CXXFLAGS) $< -o $@ $(INC)
#	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INC)

$(OBJS):%.o:%.cpp
	$(CXX) -c -fPIC $(CXXFLAGS) $< -o $@ $(INC)
#	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INC)
	
$(BIN):$(OBJS) $(UTILS)
	$(CXX) -shared -fPIC  -o $(BIN) $(LIBS) $(UTILS) $(OBJS)
#	$(CXX) -o $(BIN) $(OBJS) $(LIBS)
	mv $(BIN) ../../bin/$(BIN)
	