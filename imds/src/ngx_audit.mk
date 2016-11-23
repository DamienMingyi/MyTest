BIN := libaudit.so
INC =
LIBS=
VPATH =$(shell find ../../src -type d)

#UTILLIST=$(shell find ../../src/util/jsoncpp/src -name *.cpp)
SRCLIST=$(shell find ../../src/service/ngx_service/ngx_audit -name *.cpp)
SRCLIST+=$(shell find ../../src/util/shm -name *.cpp)
SRCLIST+=$(shell find ../../src/util/jsoncpp/src -name *.cpp)
SRCLIST+=$(shell find ../../src/util/base64 -name *.cpp)
SRCLIST+=$(shell find ../../src/public/program -name *.cpp)

OBJS=$(patsubst %.cpp,%.o,$(notdir $(SRCLIST)))
#UTILS=$(patsubst %.cpp,%.o,$(notdir $(UTILLIST)))

all:$(BIN)
	
#$(UTILS):%.o:%.cpp
#	$(CXX) -c -std=c++11 -fPIC $(CXXFLAGS) $< -o $@ $(INC)

$(OBJS):%.o:%.cpp
	$(CXX) -c -std=c++11 -fPIC $(CXXFLAGS) $< -o $@ $(INC)

$(BIN):$(OBJS)
	$(CXX) -shared -fPIC  -o $(BIN) $(LIBS)  $(OBJS)
	mv $(BIN) ../../bin/$(BIN)
