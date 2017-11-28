#include <stdio.h>
#include <apidisk.h>
#include <t2fs.h>
#include <superblock.h>
#include <disk_handler.h>
#include <fat.h>
#include <data.h>

// int main() {
//   // int i;
//   // for (i = 0; i < 11; i++) {
//   //   open2(NULL);
//   // }
//
//   // open2(NULL);
//   // open2(NULL);
//   // open2(NULL);
//   //
//   // close2(0);
//
//   int i;
//
//   init();
//   for (i = 0; i < 10; i++) {
//     int value = read_fat_entry(i);
//     printf("\nindex: %i\nvalue: %hhx\n", i, value);
//   }
//
//   //get_initial_cluster_from_path("/dir1");
//   // ls();
//   //chdir2("dir1");
//   //ls();
//   printf("%d\n", first_empty_cluster());
//
//   return 0;
// }

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
  unsigned char buffer[15];
  strcpy(buffer, "oi galero");

  int handle = open2("/testeaaa.txt");
  if (seek2(handle, 900) == 0) {
    write2(handle, buffer, sizeof(buffer));
  }

  read2(handle, buffer, sizeof(buffer));
  printf("\nCONTEUDO DO ARQUIVO:\n%s\n\n", buffer);
}

int main() {
  //test_readdir2();
  // test_readdir2();
  //test_write2();

    // create2("/carissimo_doidao.txt");
    //
    // int handle1 = opendir2("/outro_dir");
    // int handle2 = opendir2("/dir1");
    // int handle3 = opendir2("/meuzovo");
    //
    // closedir2(handle1);
    // closedir2(handle2);

//testes read2
    // FILE2 teste = open2("file1.txt");
    // FILE2 teste2 = open2("file2.txt");
    // seek2(teste2, -1);
    // close2(teste2);
    // teste2 = open2("file2.txt");
    // char buffer[10000];
    // read2(teste2, buffer, 10000);
    // printf("\n%s\n", buffer);

    // mkdir2("/outro_dir");

    // create2("/feitoria_gurizada.txt");
    // int handle = open2("/feitoria_gurizada.txt");

    // char buffer[10000];
    // getcwd2(buffer, 10);
    // printf("%s\n", buffer);
    // chdir2("dir1");
    // getcwd2(buffer, 10);
    // printf("%s\n", buffer);

 // testes truncate
  // FILE2 teste = open2("file2.txt");
  // seek2(teste, 50);
  // truncate2(teste);
  //
  // char buffer[10000];
  // seek2(teste, 0);
  // read2(teste, buffer, 10000);
  // printf("\n%s\n", buffer);

//testes delete
  //delete2("file2.txt");

  //rmdir2("dir1");
  //printf("o current path depois do rmdir2 é %s\n", current_path);




  //outro teste

  // delete2("file2.txt");
  // printf("ls no root:\n");
  // ls();
  // chdir2("dir1");
  // delete2("file1.txt");
  // printf("ls no dir:\n");
  // ls();


  rmdir2("dir1");
  chdir2("dir1");
  create2("file3.txt");
  ls();
  // delete2("file1.txt");
  // chdir2("dir1");
  // delete2("file2.txt");
  // ls();
  // chdir2("..");
  // ls();
  // rmdir2("dir1");
  // create2("testeaaa.txt");
  // chdir2("dir1/../././dir1");
  // test_write2();
  // ls();

  //struct t2fs_superbloco s;
  //readSuperBlock(&s);
  //printf("%d\n", s.NofSectors);

    return 0;
}
