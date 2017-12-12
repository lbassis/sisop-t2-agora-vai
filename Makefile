#
# Makefile ESQUELETO
#
# DEVE ter uma regra "all" para geração da biblioteca
# regra "clean" para remover todos os objetos gerados.
#
# NECESSARIO adaptar este esqueleto de makefile para suas necessidades.
#return
#

CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src
TEST_DIR=./testes

all: clean compile_to_object_file generate_static_library

compile_to_object_file:
	@echo "> Compiling object files"
	@gcc  -g -ggdb -c $(SRC_DIR)/fat.c -I $(INC_DIR) -o $(BIN_DIR)/fat.o
	@gcc  -g -ggdb -c $(SRC_DIR)/superblock.c -I $(INC_DIR) -o $(BIN_DIR)/superblock.o
	@gcc  -g -ggdb -c $(SRC_DIR)/disk_handler.c -I $(INC_DIR) -o $(BIN_DIR)/disk_handler.o
	@gcc  -g -ggdb -c $(SRC_DIR)/records_list.c -I $(INC_DIR) -o $(BIN_DIR)/records_list.o
	@gcc  -g -ggdb -c $(SRC_DIR)/t2fs.c -I $(INC_DIR) -o $(BIN_DIR)/t2fs.o

generate_static_library:
	@echo "> Generating static library"
	@ar crs $(LIB_DIR)/libt2fs.a $(BIN_DIR)/t2fs.o $(BIN_DIR)/superblock.o $(BIN_DIR)/disk_handler.o $(BIN_DIR)/fat.o $(BIN_DIR)/records_list.o
	@echo "Done!"

clean:
	@echo "> Cleaning"
	@cp $(BIN_DIR)/apidisk.o testes/
	@rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~
	@mv $(TEST_DIR)/apidisk.o $(BIN_DIR)/apidisk.o

reset_disk:
	git checkout t2fs_disk.dat

assis:
	@cat lucas.txt

run:
	@gcc -g -ggdb $(TEST_DIR)/main.c $(BIN_DIR)/apidisk.o -I $(INC_DIR) -L lib -lt2fs -o $(TEST_DIR)/main
	@./$(TEST_DIR)/main

valgrind_run:
	git checkout t2fs_disk.dat
	@gcc -g -ggdb $(TEST_DIR)/main.c $(BIN_DIR)/apidisk.o -I $(INC_DIR) -L lib -lt2fs -o $(TEST_DIR)/main
	@valgrind $(TEST_DIR)/main

shell:
	@gcc -g -ggdb exemplo/t2fstst.c $(BIN_DIR)/apidisk.o -I $(INC_DIR) -L lib -lt2fs -o exemplo/teste
	@./exemplo/teste
	
	
# exemplo:
# make test FILE=create
test:
	@gcc -g -ggdb testes/$(FILE).c $(BIN_DIR)/apidisk.o -I $(INC_DIR) -L lib -lt2fs -o testes/$(FILE)
	@echo "\n> Executando teste " $(FILE) "\n"
	@./testes/$(FILE)
	
