#include <superblock.h>
#include <t2fs.h>
#include <disk_handler.h>

RECORDS_LIST *open_files;
RECORDS_LIST *open_dirs;
int has_initialized = 0;

void init() {
    currentPath[0] = '/';
    currentPath[1] = 0;
    
    open_files = newList();
    open_dirs = newList();
}

int identify2 (char *name, int size) {
  if (!has_initialized) {
    init();
    has_initialized = 1;
  }
  
  // apenas por motivo de teste, aqui tem chamadas às funções do superblock.c
  // printf("\n== Printando diretamente do identify2 ==s\n");
  // diskId();
  // SUPERBLOCK s;
  // readSuperBlock(&s);
  // printSuperBlock(s);
}

FILE2 create2 (char *filename) {}

int delete2 (char *filename) {}

FILE2 open2 (char *filename) {
  if (!has_initialized) {
    init();
    has_initialized = 1;
  }
  
  int length = list_length(open_files);
  
  if (length >= MAX_ITEMS_IN_OPEN_LIST) {
    printf("\n=======\nLista com nro máximo de elementos, man\n=======\n");
    return -1;
  }
  
  // aqui tem que ver se o arquivo com este filename existe
  // se não existir, erro!
  // se existir, pega a entrada dele
  
  // criando um arquivo dummy
  struct t2fs_record dummy;
  
  dummy.TypeVal = 1;
  strcpy(dummy.name, "dummy_file.txt");
  dummy.bytesFileSize = 100;
  dummy.firstCluster = 4;
  
  // insere o arquivo
  insert_record(&open_files, dummy);
  
  length = list_length(open_files);
  // printf("\nAdded file: %s", dummy.name);
  // printf("\nList length: %i\n\n", length);
  
  return 0;
}

int close2 (FILE2 handle) {
  if (!has_initialized) {
    init();
    has_initialized = 1;
  }
  
  int length = list_length(open_files);
  
  if (handle < 0 || handle >= length) {
    printf("handle fora dos limites ou sei lá, man\n");
    return -1;
  }
  
  // print_records(open_files);
  // length = list_length(open_files);
  // printf("\nList length: %i\n\n", length);
  
  if (remove_record_at_index(&open_files, handle) != 0) {
    printf("Erro ao fechar arquivo %i\n", handle);
    return -1;
  }
  
  // print_records(open_files);
  // length = list_length(open_files);
  // printf("\nList length: %i\n\n", length);
  
  return 0;
}

int read2 (FILE2 handle, char *buffer, int size) {
  if (!has_initialized) {
    init();
    has_initialized = 1;
  }
  
  if (handle < 0 || handle > MAX_ITEMS_IN_OPEN_LIST) {
    printf("handle fora dos limites, man\n");
    return -1;
  }
  struct t2fs_record *rec;
  rec = (struct t2fs_record *) get_record_at_index(open_files, 0);
  
  if (rec == NULL) {
    printf("O handle %i non ecziste\n", handle);
    return -1;
  }
  
  // achou o arquivos
  printf("Reading %s\n", rec->name);
  
  // aqui falta colocar o conteúdo do arquivo no buffer, creio eu
  // (ler "size" bytes a partir do current_pointer)
  
  return 0;
}

int write2 (FILE2 handle, char *buffer, int size) {}

int truncate2 (FILE2 handle) {}

int seek2 (FILE2 handle, unsigned int offset) {}

int mkdir2 (char *pathname) {}

int rmdir2 (char *pathname) {}

int chdir2 (char *pathname) {}

int getcwd2 (char *pathname, int size) {}

DIR2 opendir2 (char *pathname) {}

int readdir2 (DIR2 handle, DIRENT2 *dentry) {}

int closedir2 (DIR2 handle) {}
