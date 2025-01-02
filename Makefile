bin/MtFuji: bin/func1.o bin/mtfuji.o 
	gcc bin/func1.o bin/mtfuji.o -o bin/MtFuji

bin/func1.o: src/func1.c
	gcc -c -I./include src/func1.c -o bin/func1.o

bin/mtfuji.o: src/mtfuji.c
	gcc -c -I./include src/mtfuji.c -o bin/mtfuji.o -lm

PHONY: clean

clean:
	rm bin/*