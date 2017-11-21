#include <superblock.h>
#include <t2fs.h>


int identify2 (char *name, int size) {
  // apenas por motivo de teste, aqui tem chamadas às funções do superblock.c
  printf("\n== Printando diretamente do identify2 ==s\n");
  diskId();
  SUPERBLOCK s;
  readSuperBlock(&s);
  printSuperBlock(s);
}

FILE2 create2 (char *filename) {}

int delete2 (char *filename) {}

FILE2 open2 (char *filename) {}

int close2 (FILE2 handle) {}

int read2 (FILE2 handle, char *buffer, int size) {}

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
