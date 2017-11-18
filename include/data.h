#ifndef __data__
#define __data__

#include <stdio.h>


#define DIR_ENTRY_OFFSET 0
#define DIR_ENTRY_SIZE 1

#define DIR_NAME_OFFSET 1
#define DIR_NAME_SIZE 55

#define DIR_BYTES_FILE_SIZE_OFFSET 56
#define DIR_BYTES_FILE_SIZE_SIZE 4

#define DIR_FIRST_CLUSTER_OFFSET 60
#define DIR_FIRST_CLUSTER_SIZE 4


typedef struct DIRECTORY {
  BYTE typeVal
  char name[55];
  unsigned int bytesFileSize;
  unsigned int firstCluster;
}DIRECTORY;


#endif