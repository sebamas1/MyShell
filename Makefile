CFLAGS = -g -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11

myshell: obj/myshell.o obj/internalCommands.o obj/LinkedList.o obj/programInvocation.o obj/parseIO.o
	gcc $(CFLAGS) -o myshell $^
	
obj/myshell.o: bin/myshell.c 
	mkdir -p obj
	cppcheck --enable=all ./bin #style,performance,portability
	gcc $(CFLAGS) -c bin/myshell.c -o obj/myshell.o
	
obj/internalCommands.o: bin/InCommands/internalCommands.h bin/InCommands/internalCommands.c
	gcc $(CFLAGS) -c bin/InCommands/internalCommands.c -o obj/internalCommands.o
	
obj/LinkedList.o: bin/util/LinkedList.h bin/util/LinkedList.c
	gcc $(CFLAGS) -c bin/util/LinkedList.c -o obj/LinkedList.o
	
obj/programInvocation.o: bin/ProgramParsing/programInvocation.h bin/ProgramParsing/programInvocation.c
	gcc $(CFLAGS) -c bin/ProgramParsing/programInvocation.c -o obj/programInvocation.o
	
obj/parseIO.o: bin/ProgramParsing/parseIO.h bin/ProgramParsing/parseIO.c
	gcc $(CFLAGS) -c bin/ProgramParsing/parseIO.c -o obj/parseIO.o

clean:
	rm -f obj/*.o
	rmdir obj