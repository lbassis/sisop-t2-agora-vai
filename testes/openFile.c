#include <stdio.h>
#include <apidisk.h>
#include <t2fs.h>
#include <superblock.h>
#include <disk_handler.h>
#include <fat.h>
#include <data.h>

int main() {
  int handle0 = open2("dir1/arquivo0.txt");
  int handle1 = open2("dir1/arquivo1.txt");
  int handle2 = open2("dir1/arquivo2.txt");
  int handle3 = open2("dir1/arquivo3.txt");
  int handle4 = open2("dir1/arquivo4.txt");
  int handle5 = open2("dir1/arquivo5.txt");
  int handle6 = open2("dir1/arquivo6.txt");
  int handle7 = open2("dir1/arquivo7.txt");
  int handle8 = open2("dir1/arquivo8.txt");
  int handle9 = open2("dir1/arquivo9.txt");
  int handle10 = open2("dir1/arquivo10.txt");
  
  close2(handle8);
  
  handle10 = open2("dir1/arquivo10.txt");
  
  return 0;
}
