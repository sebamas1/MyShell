CFLAGS = -Wall -pedantic

myshell: obj/myshell.o obj/internalCommands.o obj/LinkedList.o obj/programExecution.o obj/parseIO.o obj/programParsing.o obj/signalHandling.o obj/parsePipes.o
	gcc $(CFLAGS) -o myshell $^
	
obj/myshell.o: bin/myshell.c 
	mkdir -p obj
	cppcheck --enable=all ./bin #style,performance,portability
	gcc $(CFLAGS) -c bin/myshell.c -o obj/myshell.o
	
obj/internalCommands.o: bin/InCommands/internalCommands.h bin/InCommands/internalCommands.c
	gcc $(CFLAGS) -c bin/InCommands/internalCommands.c -o obj/internalCommands.o
	
obj/LinkedList.o: bin/util/LinkedList.h bin/util/LinkedList.c
	gcc $(CFLAGS) -c bin/util/LinkedList.c -o obj/LinkedList.o
	
obj/programExecution.o: bin/ProgramParsing/programExecution.h bin/ProgramParsing/programExecution.c
	gcc $(CFLAGS) -c bin/ProgramParsing/programExecution.c -o obj/programExecution.o
	
obj/parseIO.o: bin/ProgramParsing/parseIO.h bin/ProgramParsing/parseIO.c
	gcc $(CFLAGS) -c bin/ProgramParsing/parseIO.c -o obj/parseIO.o
	
obj/programParsing.o : bin/ProgramParsing/programParsing.h bin/ProgramParsing/programParsing.c
	gcc $(CFLAGS) -c bin/ProgramParsing/programParsing.c -o obj/programParsing.o

obj/signalHandling.o : bin/SignalHandling/signalHandling.h bin/SignalHandling/signalHandling.c
	gcc $(CFLAGS) -c bin/SignalHandling/signalHandling.c -o obj/signalHandling.o
	
obj/parsePipes.o : bin/ProgramParsing/parsePipes.h bin/ProgramParsing/parsePipes.c
	gcc $(CFLAGS) -c bin/ProgramParsing/parsePipes.c -o obj/parsePipes.o

clean:
	rm -f obj/*.o
	rmdir obj