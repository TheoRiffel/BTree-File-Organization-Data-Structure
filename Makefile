CC            := gcc

BIN           := ./bin
OBJ           := ./obj
INCLUDE       := ./include
SRC           := ./src
SRC_FILES     := $(wildcard $(SRC)/*.c)
OBJ_FILES     := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRC_FILES))

PROGRAM       := $(BIN)/main

CFLAGS        := -I$(INCLUDE) -g
LEAKFLAGS     := --leak-check=full --show-leak-kinds=all --track-origins=yes 

rem:
	rm *.bin
cpy:
	cp arquivos/antes/* ./

all: $(PROGRAM)

$(PROGRAM): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(PROGRAM) all 
	$<

clear:
	clear
clean: rem
	rm -rf $(PROGRAM) $(OBJ_FILES)

zip:
	zip -r main.zip Makefile src includes bin obj -x "*/*.o" -x "bin/main"

time: CFLAGS += -DTIME
time: $(PROGRAM)
	$<

valgrind: CFLAGS += -g
valgrind: $(PROGRAM) all rem cpy
	valgrind $(LEAKFLAGS) $(PROGRAM)
