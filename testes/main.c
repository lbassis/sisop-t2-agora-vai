#include <stdio.h>
#include <apidisk.h>
#include <t2fs.h>
#include <superblock.h>
#include <disk_handler.h>
#include <stdint.h>

void read_fat() {
  char buffer[SECTOR_SIZE];
  read_sector(1, buffer);

  int i = 0;
  for (i = 0; i < 30; i++) {
    int offset = i * 4;
    unsigned int value;

    value = (buffer[3 + offset] << 24 | buffer[2 + offset] << 16 | buffer[1 + offset] << 8 | buffer[0 + offset]);
    printf("fat %i => %hhx\n", i, value);
  }
  // print_sector(1);
}

int main() {
  read_all_records(2);
  // identify2(NULL, 0);

  //read_fat();

  return 0;
}
