#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ID_SIZE 4
#define DISK_FILE "t2fs_disk.dat"




#define SUPER_BLOCK_SIZE_OFFSET 6
#define SUPER_BLOCK_SIZE 2


int hexToInt(unsigned char *result) {

  int value = (result[0] << 8 | result[1]);
  return value;
}


void diskId() { // só printa mesmo

  char id[ID_SIZE+1];
  FILE *file = fopen(DISK_FILE, "r");

  fseek(file, 0, SEEK_SET);

  fread(id, 1, ID_SIZE, file);
  id[ID_SIZE] = (char) 0; // marca de final da string
  printf("ID: %s\n", id);

  fclose(file);
}

int superBlockSize() {

  unsigned char buffer[SUPER_BLOCK_SIZE];
  FILE *file = fopen(DISK_FILE, "r");

  fseek(file, 6, SEEK_SET);

  fread(buffer, 1, SUPER_BLOCK_SIZE, file);

  return hexToInt(buffer);
}

int main() {

  diskId();


}
