CFLAGS = -g -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11

myshell: obj/myshell.o obj/internalCommands.o obj/LinkedList.o obj/programInvocation.o
	gcc $(CFLAGS) -o myshell $^
	
obj/myshell.o: bin/myshell.c 
	mkdir -p obj
	cppcheck --enable=style,performance,portability ./bin
	gcc $(CFLAGS) -c bin/myshell.c -o obj/myshell.o
	
obj/internalCommands.o: bin/InCommands/internalCommands.h bin/InCommands/internalCommands.c
	gcc $(CFLAGS) -c bin/InCommands/internalCommands.c -o obj/internalCommands.o
	
obj/LinkedList.o: bin/util/LinkedList.h bin/util/LinkedList.c
	gcc $(CFLAGS) -c bin/util/LinkedList.c -o obj/LinkedList.o
	
obj/programInvocation.o: bin/ProgramInvocation/programInvocation.h bin/ProgramInvocation/programInvocation.c
	gcc $(CFLAGS) -c bin/ProgramInvocation/programInvocation.c -o obj/programInvocation.o

clean:
	rm -f obj/*.o
	rmdir obj