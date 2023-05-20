/*#########################_Pier_Luigi_Manfrini_############################
 *
 *	Debug and Management for solution08
 *
 *	This program creates a large txt file named "large_txt" of the size 
 *	specified by CLI. (size is provided in MegaBytes Mb)
 *
 *	Each byte is a random char from the ascii table (0~127 in decimal)
 *
 *	Usage:
 *		<./create_large_txt> <Mb>
 *	
 *	Note:
 *		- max size is 2GB ~ (2000Mb) 
 *
 * 	Compile:
 * 		gcc create_large_txt.c -o create_large_txt
 * 					 -Wall -Werror -fsanitize=leak
 *
 * #########################################################################
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    dprintf(2, "Wrong Usage: <./create_large_txt> <size(Mb)>\n");
    exit(EXIT_FAILURE);
  }
  if((atoi(argv[1]))>2000){
	dprintf(2, "Max allowed size is 2GB ~ (2000Mb)\n");
	exit(EXIT_FAILURE);
  } 
  int out_file =
      open("large_txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  size_t size = (size_t)atoi(argv[1]) * 1024 * 1024;
  srand((unsigned int)time(NULL));
  char *file = (char *)malloc(size);
  for (int i = 0; i < size; i++) {
    file[i] = (int)rand() % 128;
  }	
  if(write(out_file,file,size)!=size)
	  perror("");
  free(file);
  close(out_file);
  return 0;
}
