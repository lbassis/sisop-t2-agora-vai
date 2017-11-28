#ifndef __disk_handler__
#define __disk_handler__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <apidisk.h>
#include <t2fs.h>
#include <disk_handler.h>
#include <records_list.h>


void read_cluster(int cluster_index, char *buffer);
struct t2fs_record read_record(char *buffer, int start);
void read_all_records(int cluster_index, RECORDS_LIST **records);

void print_sector(int sector);
void print_cluster(char *buffer);
void print_record(struct t2fs_record record);

struct t2fs_record *find_record(RECORDS_LIST *list, char *name);

int get_record_initial_cluster(struct t2fs_record record);

int get_initial_cluster_from_path(char *path);

unsigned int first_empty_cluster();

void ls();

char *get_father_dir_path(char *path);

int write_list_of_records_to_cluster(RECORDS_LIST *list, int cluster_index);

#endif
