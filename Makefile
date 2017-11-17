#
# Makefile ESQUELETO
#
# DEVE ter uma regra "all" para geração da biblioteca
# regra "clean" para remover todos os objetos gerados.
#
# NECESSARIO adaptar este esqueleto de makefile para suas necessidades.
#
#

CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src
TEST_DIR=./testes

all:

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~

assis:
	@cat lucas.txt
	
run:
	@gcc -g -c $(SRC_DIR)/superblock.c -I include -o $(BIN_DIR)/superblock.o
	@gcc -g $(TEST_DIR)/main.c $(BIN_DIR)/superblock.o -I include -o $(TEST_DIR)/main
	@./$(TEST_DIR)/main
