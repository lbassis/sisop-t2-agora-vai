
#ifndef __records_list__
#define __records_list__

#include <stdio.h>
#include <stdlib.h>
#include <t2fs.h>

typedef struct file_or_dir_record {
	struct t2fs_record record;
	int pointer;
	int handler;
	char *path;
} GENERIC_FILE;

typedef struct RECORDS_LIST {
  GENERIC_FILE generic_file;
  struct RECORDS_LIST *next;
} RECORDS_LIST;

RECORDS_LIST *newList();
void insert_record(RECORDS_LIST **q, GENERIC_FILE generic_file);
void print_records(RECORDS_LIST *q);
int list_length(RECORDS_LIST *q);
GENERIC_FILE *get_record_at_index(RECORDS_LIST *q, int index);
int remove_record_at_index(RECORDS_LIST **q, int handler_to_remove);
int get_fisrt_handler_available(RECORDS_LIST *list, int list_max_items);

#endif
