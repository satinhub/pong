FLAG = -Werror -Wextra -Wall
 
all : pong

pong : pong.o 
	gcc $(FLAG) build/pong.o -o build/pong

pong.o : pong.c
	gcc $(FLAG) -c pong.c -o build/pong.o


clean : 
	rm -rf build/*.o 
	rm -rf build/pong
	rm -rf a.out

rebuild :
	make clean
	make all

leaks :
	leaks -atExit -- build/pong

clang: 
	clang-format -i *.c

cpp :
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
