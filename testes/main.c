#include <stdio.h>
#include <apidisk.h>
#include <t2fs.h>
#include <superblock.h>
#include <disk_handler.h>
#include <fat.h>
#include <data.h>

void print_dirent2(DIRENT2 *dir) {
  printf("name: %s\n", dir->name);
  printf("fileType: %i\n", dir->fileType);
  printf("fileSize: %i\n", dir->fileSize);
  printf("---------------------\n");
}

void test_readdir2() {
  int handle_dir1 = opendir2("/dir1");
  // create2("/dir1/carissimo_doidao.txt");

  DIRENT2 *dir1 = malloc(sizeof(DIRENT2));

  printf("=== Testando a readdir2 em /dir1 ===\n\n------------------------\n");

  int i;
  for (i = 0; i < 6; i++) {
    int error = readdir2(handle_dir1, dir1);
    if (!error) {
      print_dirent2(dir1);
    }
  }

  printf("\n");
}

void test_write2() {
  create2("/testeaaa.txt");

  unsigned char buffer[30];
  unsigned char another_buffer[50];

  strcpy(buffer, "oi galero");

  int handle = open2("testeaaa.txt");

  write2(handle, buffer, sizeof(buffer));
  seek2(handle, 9);

  strcpy(buffer, ", como vai o6?");
  write2(handle, buffer, sizeof(buffer));
  seek2(handle, 0);

  read2(handle, another_buffer, sizeof(another_buffer));
  printf("\n-> Conteudo do arquivo:\n%s\n\n", another_buffer);
}

int main() {
  test_write2();
  chdir2("..");
  ls();
    return 0;
}
