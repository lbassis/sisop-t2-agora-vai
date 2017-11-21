
#ifndef __records_list__
#define __records_list__

#include <stdio.h>
#include <stdlib.h>
#include <t2fs.h>

typedef struct RECORDS_LIST {
  struct t2fs_record record;
  struct RECORDS_LIST *next;
} RECORDS_LIST;

RECORDS_LIST *newList();
void insert_record(RECORDS_LIST **l, struct t2fs_record record);
void print_records(RECORDS_LIST *q);

#endif
