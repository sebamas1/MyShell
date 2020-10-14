CFLAGS = -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11

myshell: obj/myshell.o obj/internalCommands.o
	gcc $(CFLAGS) -o myshell $^
	
obj/myshell.o: bin/myshell.c 
	mkdir -p obj
	cppcheck --enable=style,performance,portability ./bin
	gcc $(CFLAGS) -c bin/myshell.c -o obj/myshell.o
	
obj/internalCommands.o: bin/InCommands/internalCommands.h bin/InCommands/internalCommands.c
	gcc $(CFLAGS) -c bin/InCommands/internalCommands.c -o obj/internalCommands.o
	
clean:
	rm -f obj/*.o
	rmdir obj