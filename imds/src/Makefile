CXX = gcc
VER = Debug
ifeq ($(VER), Debug)
	CXXFLAGS := -g -Wall -D Debug
else
	CXXFLAGS := -g -Wall -O3
endif
export CXXFLAGS	

SRCLIST = $(shell find ../src -name *.cpp)

all: main

main: rebuilder shmcreate initial transfer close service loader history
initial: shoinitial sh1initial sz1v5initial sh2initial shhkinitial csiinitial

transfer: shotransfer sh1transfer sz1v5transfer

close: shoclose

service: libmqws libauth

loader: historyload

history: sh1historysave sh2historysave

shmcreate:
	cd ../debug/shmcreater && $(MAKE) -f ../../src/shmcreater.mk
	
rebuilder:
	cd ../debug/rebuild && $(MAKE) -f ../../src/rebuild.mk
	
shoinitial:
	cd ../debug/sho && $(MAKE) -f ../../src/shoinitial.mk

sh1transfer:
	cd ../debug/sh1 && $(MAKE) -f ../../src/sh1transfer.mk

sz1v5transfer:
	cd ../debug/sz1v5 && $(MAKE) -f ../../src/sz1v5transfer.mk
	
shotransfer:
	cd ../debug/sho && $(MAKE) -f ../../src/shotransfer.mk
	
shoclose:
	cd ../debug/sho && $(MAKE) -f ../../src/shoclose.mk

sh1initial:
	cd ../debug/sh1 && $(MAKE) -f ../../src/sh1initial.mk

sh1list:
	cd ../debug/sh1 && $(MAKE) -f ../../src/sh1list.mk

sh2initial:
	cd ../debug/sh2 && $(MAKE) -f ../../src/sh2initial.mk

sh2list:
	cd ../debug/sh2 && $(MAKE) -f ../../src/sh2list.mk
	
sz1v5initial:
	cd ../debug/sz1v5 && $(MAKE) -f ../../src/sz1v5initial.mk

sz1v5list:
	cd ../debug/sz1v5 && $(MAKE) -f ../../src/sz1v5list.mk
	
cninitial:
	cd ../debug/cn && $(MAKE) -f ../../src/cninitial.mk
	
cnlist:
	cd ../debug/cn && $(MAKE) -f ../../src/cnlist.mk
	
shhkinitial :
	cd ../debug/shhk && $(MAKE) -f ../../src/shhkinitial.mk

csiinitial:
	cd ../debug/csi && $(MAKE) -f ../../src/csiinitial.mk

libshm:
	@if [ ! -d ../debug/util ]; then mkdir ../debug/util; fi
	cd ../debug/util && $(MAKE) -f ../../src/shm.mk
	
libmqws: libshm
	@if [ ! -d ../debug/libmqws ]; then mkdir ../debug/libmqws; fi
	cd ../debug/libmqws && $(MAKE) -f ../../src/ngx_mqws.mk

libauth:
	cd ../debug/libauth && $(MAKE) -f ../../src/ngx_auth.mk
	
libaudit:
	cd ../debug/libaudit && $(MAKE) -f ../../src/ngx_audit.mk

historyload:
	cd ../debug/loader && $(MAKE) -f ../../src/historyload.mk
	
sh1historysave:
	cd ../debug/sh1 && $(MAKE) -f ../../src/sh1historysave.mk

sh2historysave:
	cd ../debug/sh2 && $(MAKE) -f ../../src/sh2historysave.mk

.PHONY: clean
clean:
	cd ../debug/rebuild && rm -f *.o
	cd ../debug/shmcreater && rm -f *.o
	cd ../debug/sho && rm -f *.o
	cd ../debug/sh1 && rm -f *.o
	cd ../debug/sh2 && rm -f *.o
	cd ../debug/sz1v5 && rm -f *.o
	cd ../debug/hkp && rm -f *.o
	cd ../debug/csi && rm -f *.o
	cd ../debug/util && rm -f *.o
	cd ../debug/libmqws && rm -f *.o
	cd ../debug/libauth && rm -f *.o
	cd ../debug/loader && rm -f *.o
