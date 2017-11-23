#include <stdio.h>
#include <apidisk.h>
#include <t2fs.h>
#include <superblock.h>
#include <disk_handler.h>
#include <fat.h>

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

//
// testando lista de GENERIC_FILE
//
int main() {
    open2(NULL);
    open2(NULL);
    open2(NULL);

    close2(0);
    
    return 0;
}