#include <stdio.h>
#include <apidisk.h>
#include <stdint.h>
#include <fat.h>

int read_fat_entry(int index) {
  if (index < 0 || index > NUMBER_OF_FAT_ENTRIES) {
    return -1;
  }

  int index_copy = index, sector_index = 1;
  
  while (index_copy > 0) {
    if (index_copy >= 64) {
      index_copy -= 64;
      sector_index ++;
    } else {
      break;
    }
  }
  
  char buffer[SECTOR_SIZE];
  read_sector(sector_index, buffer);
  
  int fat_index_in_sector = index - ((sector_index - 1) * 64);
  int value, offset = fat_index_in_sector * 4;
  
  value = (buffer[3 + offset] << 24 | buffer[2 + offset] << 16 | buffer[1 + offset] << 8 | buffer[0 + offset]);
  
  return value;
}
