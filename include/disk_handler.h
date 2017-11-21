#ifndef __disk_handler__
#define __disk_handler__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <apidisk.h>
#include <t2fs.h>
#include <disk_handler.h>
#include <records_list.h>


#define DATA_SECTOR_START 129

#define SECTORS_PER_CLUSTER 4

#define CLUSTER_SIZE 1024


void read_cluster(int cluster_index, char *buffer);
struct t2fs_record read_record(char *buffer, int start);
void read_all_records(int cluster_index, RECORDS_LIST **records);

void print_sector(int sector);
void print_cluster(char *buffer);
void print_record(struct t2fs_record record);

#endif
