#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ID_SIZE 4
#define DISK_FILE "t2fs_disk.dat"




#define SUPER_BLOCK_SIZE_OFFSET 6
#define SUPER_BLOCK_SIZE 2


void readElement(FILE *file, int size, char *result) {

  int i = 0;

  while (i<size) {
    fread(&result[i], 1, 1, file);
    printf("leu o %d\n", result[i]);
    i++;
  }
}

int hexToInt(char *hex) {
  int number = (int)strtol(hex, NULL, 16);
  return number;
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

  char buffer[SUPER_BLOCK_SIZE];
  FILE *file = fopen(DISK_FILE, "r");

  fseek(file, SUPER_BLOCK_SIZE_OFFSET, SEEK_SET);

  readElement(file, SUPER_BLOCK_SIZE, buffer);
  printf("%s\n", buffer);
  //fread(buffer, 1, SUPER_BLOCK_SIZE, file);
  
  //buffer[SUPER_BLOCK_SIZE] = (char) 0; // marca de final da string
  // printf("hex: %s\n", buffer);
  //printf("SuperBlockSize: %d\n", hexToInt(buffer));
  //printf("%d\n", hexToInt("10"));
  fclose(file);

  return 0;
}

int main() {

   diskId();
  superBlockSize();
}
