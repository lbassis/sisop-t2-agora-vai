#include <stdio.h>
#include <apidisk.h>
#include <t2fs.h>
#include <superblock.h>
#include <disk_handler.h>
#include <fat.h>
#include <data.h>

int main() {
  unsigned char buffer[200], another_buffer[200];
  strcpy(buffer, "An operational system is a term used in data warehousing to refer to a system that is used to process the day-to-day transactions of an organization.");
  
  create2("sisop.txt");
  int handle = open2("sisop.txt");
  
  write2(handle, buffer, sizeof(buffer));
  
  seek2(handle, 0);
  
  read2(handle, another_buffer, sizeof(another_buffer));
  
  printf("\n-> Conteúdo do arquivo:\n%s\n\n", another_buffer);
    
  return 0;
}
