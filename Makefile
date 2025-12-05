# Makefile for university module
# "make clean" deletes the executable to build again
# "make test" builds the main file and then runs the test script.

main.out:
	gcc main.c funcs.c -o main.out -lm

clean:
	-rm main.out

test: clean main.out
	bash test.sh
