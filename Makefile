SRC = src/
OUT = .elf
DEPENDENCY = $(SRC)imageprocessing.c $(SRC)filter.c
LIBRARY = $(SRC).
FLAGS = -lfreeimage

single:
	gcc -I$(LIBRARY) $(SRC)main_single.c $(DEPENDENCY) -o main_single$(OUT) $(FLAGS)

process:
	gcc -I$(LIBRARY) $(SRC)main_process.c $(DEPENDENCY) -o main_process$(OUT) $(FLAGS)

threading:
	gcc -I$(LIBRARY) $(SRC)main_threading.c $(DEPENDENCY) -o main_threading$(OUT) $(FLAGS) -lpthread

test: single process threading
	sh test.sh

clean:
	rm *.elf
