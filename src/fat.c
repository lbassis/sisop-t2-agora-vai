#include <stdio.h>
#include <apidisk.h>
#include <stdint.h>
#include <fat.h>

int get_first_fat_entry_available() {
  
  int i, fat_entry;
  
  for (i = FIRST_USABLE_FAT_ENTRY; i < NUMBER_OF_FAT_ENTRIES; i++) {
    fat_entry = read_fat_entry(i);
    // printf("%i: \t", i);
    // printf("%hhx\t\n", fat_entry);
    // printf("%i\t\n\n", fat_entry);
    
    if (fat_entry == 0) {
      return i;
    }
  }
  
  return -1;
}

int read_fat_entry(int index) {
  if (index < 0 || index > NUMBER_OF_FAT_ENTRIES) {
    return -1;
  }

  int sector_index = find_in_which_sector_fat_entry_is(index);
  
  char buffer[SECTOR_SIZE];
  read_sector(sector_index, buffer);
  
  int fat_index_in_sector = index - ((sector_index - 1) * 64);
  int value, offset = fat_index_in_sector * 4;
  
  // printf("\n%i => 0x%hhx%hhx%hhx%hhx\n", index, buffer[3 + offset], buffer[2 + offset], buffer[1 + offset], buffer[offset]);
  value = (buffer[3 + offset] << 24 | buffer[2 + offset] << 16 | buffer[1 + offset] << 8 | buffer[0 + offset]);
  
  return value;
}

int set_fat_entry(int index, int value) {
  if (index < 0 || index > NUMBER_OF_FAT_ENTRIES) {
    return -1;
  }

  int sector_index = find_in_which_sector_fat_entry_is(index);
  
  char buffer[SECTOR_SIZE];
  read_sector(sector_index, buffer);

  int fat_index_in_sector = index - ((sector_index - 1) * 64);
  int offset = fat_index_in_sector * 4;
  
  // printf("Value to be put: %i\n", value);
  
  // escrever no lugar certo do buffer o novo valor
  buffer[offset + 3] = (value >> 24);
  buffer[offset + 2] = (value >> 16);
  buffer[offset + 1] = (value >> 8);
  buffer[offset] = value;
  
  // printf("%i => %hhx %hhx %hhx %hhx\n", value,
  //                       buffer[offset + 3],
  //                       buffer[offset + 2],
  //                       buffer[offset + 1],
  //                       buffer[offset]);

  // escreve no setor
  write_sector(sector_index, buffer);

  return 0;
}

int find_in_which_sector_fat_entry_is(int index) {
  int sector_index = 1;

  while (index > 0) {
    if (index >= 64) {
      index -= 64;
      sector_index ++;
    } else {
      break;
    }
  }

  return sector_index;
}

int print_fat() {
  int i, fat_entry;
  
  for (i = FIRST_USABLE_FAT_ENTRY; i < NUMBER_OF_FAT_ENTRIES; i++) {
    fat_entry = read_fat_entry(i);
    // printf("%i: \t", i);
    // printf("%hhx\t", fat_entry);
    // printf("%i\t\n\n", fat_entry);
  }
  
  return 0;
}

int ceil2(double number) {
  int ceiled = (int) number;
  
  if (ceiled < number) {
    ceiled += 1;
  }
  
  return ceiled;
}

int floor2(double number) {
  return (int) number;
}
