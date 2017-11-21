#include <stdio.h>
#include <apidisk.h>
#include <t2fs.h>
#include <superblock.h>
#include <disk_handler.h>
#include <fat.h>


int main() {
  int i;
  for (i = 0; i < 10; i++) {
    int value = read_fat_entry(i);
    printf("\nindex: %i\nvalue: %hhx\n", i, value);
  }
  
  return 0;
}
