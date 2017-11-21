#ifndef __fat__
#define __fat__

#include <stdio.h>
#include <apidisk.h>
#include <stdint.h>

#define NUMBER_OF_FAT_ENTRIES 8192 // (256 * 128) / 4


int read_fat_entry(int index);


#endif
