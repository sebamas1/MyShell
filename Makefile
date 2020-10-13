CFLAGS = -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11

myshell: obj/myshell.o
	gcc $(CFLAGS) -o myshell $^
	
obj/myshell.o: bin/myshell.c 
	mkdir -p obj
	cppcheck --enable=style,performance,portability ./bin
	gcc $(CFLAGS) -c bin/myshell.c -o obj/myshell.o
	
clean:
	rm -f obj/*.o
	rmdir obj