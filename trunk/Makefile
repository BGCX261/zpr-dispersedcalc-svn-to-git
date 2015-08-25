CC=g++
CFLAGS= -I/usr/local/include/boost-1_37/ -lboost_threads -liqxmlrpc
LDDIRS=libs/dispcalc
CALCLIBS= $(LDDIRS)/TextUserInterface.o          \
	  $(LDDIRS)/ClientController.o           \
	  $(LDDIRS)/ClientControllerActions.o    \
	  $(LDDIRS)/Parser.o		         \
	  $(LDDIRS)/ClientControllerSupervisor.o \
	  $(LDDIRS)/ClientControllerTask.o       \

CALSLIBS= $(LDDIRS)/ServerController.o           \
	  $(LDDIRS)/ServerMethods.o

all: client server

client: client_test.o calcdir
	$(CC) $(CFLAGS) client_test.o $(CALCLIBS)  -o client_test

server: server_test.o calcdir
	$(CC) $(CFLAGS) server_test.o $(CALSLIBS) -o server_test

server_test.o: server_test.cpp
	$(CC) $(CFLAGS) -c server_test.cpp

client_test.o: client_test.cpp ${LDDIRS}/controller/ClientController.h 
	$(CC) $(CFLAGS) -c client_test.cpp 

calcdir: 
	for dir in ${LDDIRS}; do ( cd $$dir; ${MAKE} all); done




clean:
	rm *.o; \
	for dir in ${LDDIRS}; do ( cd $$dir; ${MAKE} clean); done
