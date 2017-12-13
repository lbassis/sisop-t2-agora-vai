#include <stdio.h>
#include <apidisk.h>
#include <t2fs.h>
#include <superblock.h>
#include <disk_handler.h>
#include <fat.h>
#include <data.h>

int main() {
  create2("dir1/arquivo0.txt");
  create2("dir1/arquivo1.txt");
  create2("dir1/arquivo2.txt");
  create2("dir1/arquivo3.txt");
  create2("dir1/arquivo4.txt");
  create2("dir1/arquivo5.txt");
  create2("dir1/arquivo6.txt");
  create2("dir1/arquivo7.txt");
  create2("dir1/arquivo8.txt");
  create2("dir1/arquivo9.txt");
  create2("dir1/arquivo10.txt");
  
  mkdir2("dir0");
  mkdir2("dir1");
  mkdir2("dir2");
  mkdir2("dir3");
  mkdir2("dir4");
  mkdir2("dir5");
  mkdir2("dir6");
  mkdir2("dir7");
  mkdir2("dir8");
  mkdir2("dir9");
  mkdir2("dir10");
  
  return 0;
}
