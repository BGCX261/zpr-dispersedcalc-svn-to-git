CC=g++
CFLAGS=-c
CLIBS= -I/usr/local/include/boost-1_37/

all: TextUserInterface.o ClientController.o ClientControllerActions.o  \
     Parser.o ClientControllerSupervisor.o ClientControllerTask.o      \
     ServerController.o ServerMethods.o

TextUserInterface.o: TextUserInterface.cpp userinterface/TextUserInterface.h
	$(CC) $(CFLAGS) TextUserInterface.cpp $(CLIBS)

ClientController.o: ClientController.cpp controller/ClientController.h
	$(CC) $(CFLAGS) ClientController.cpp $(CLIBS)

ClientControllerActions.o: ClientControllerActions.cpp controller/ClientControllerActions.h
	$(CC) $(CFLAGS) ClientControllerActions.cpp $(CLIBS)

Parser.o: Parser.cpp parser/Parser.h
	$(CC) $(CFLAGS) Parser.cpp $(CLIBS)

ClientControllerSupervisor.o: ClientControllerSupervisor.cpp controller/ClientControllerSupervisor.h
	$(CC) $(CFLAGS) ClientControllerSupervisor.cpp $(CLIBS)

ClientControllerTask.o: ClientControllerTask.cpp controller/ClientControllerTask.h
	$(CC) $(CFLAGS) ClientControllerTask.cpp $(CLIBS)

ServerController.o: ServerController.cpp controller/ServerController.h
	$(CC) $(CFLAGS) ServerController.cpp $(CLIBS)

ServerMethods.o: ServerMethods.cpp server/ServerMethods.h
	$(CC) $(CFLAGS) ServerMethods.cpp $(CLIBS)

clean:
	rm *.o
