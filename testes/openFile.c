#include <stdio.h>
#include <apidisk.h>
#include <t2fs.h>
#include <superblock.h>
#include <disk_handler.h>
#include <fat.h>
#include <data.h>

int main() {
  create2("arquivo0.txt");
  create2("arquivo1.txt");
  create2("arquivo2.txt");
  create2("arquivo3.txt");
  create2("arquivo4.txt");
  create2("arquivo5.txt");
  create2("arquivo6.txt");
  create2("arquivo7.txt");
  create2("arquivo8.txt");
  create2("arquivo9.txt");
  create2("arquivo10.txt");
  
  int handle0 = open2("arquivo0.txt");
  int handle1 = open2("arquivo1.txt");
  int handle2 = open2("arquivo2.txt");
  int handle3 = open2("arquivo3.txt");
  int handle4 = open2("arquivo4.txt");
  int handle5 = open2("arquivo5.txt");
  int handle6 = open2("arquivo6.txt");
  int handle7 = open2("arquivo7.txt");
  int handle8 = open2("arquivo8.txt");
  int handle9 = open2("arquivo9.txt");
  int handle10 = open2("arquivo10.txt");
  
  return 0;
}
