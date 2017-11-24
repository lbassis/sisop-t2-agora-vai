#ifndef __fat__
#define __fat__

#include <stdio.h>
#include <apidisk.h>
#include <stdint.h>

#define NUMBER_OF_FAT_ENTRIES 8192 // (256 * 128) / 4
#define FIRST_USABLE_FAT_ENTRY 2 // (256 * 128) / 4


int read_fat_entry(int index);
int set_fat_entry(int index, int value);
int get_first_fat_entry_available();
int find_in_which_sector_fat_entry_is(int index);

#endif
