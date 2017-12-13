#include <stdio.h>
#include <apidisk.h>
#include <t2fs.h>
#include <superblock.h>
#include <disk_handler.h>
#include <fat.h>
#include <data.h>

int main() {
  int handle0 = opendir2("dir0");
  int handle1 = opendir2("dir1");
  int handle2 = opendir2("dir2");
  int handle3 = opendir2("dir3");
  int handle4 = opendir2("dir4");
  int handle5 = opendir2("dir5");
  int handle6 = opendir2("dir6");
  int handle7 = opendir2("dir7");
  int handle8 = opendir2("dir8");
  int handle9 = opendir2("dir9");
  int handle10 = opendir2("dir10");
  
  closedir2(handle8);
  
  handle10 = opendir2("dir10");
  
  return 0;
}
