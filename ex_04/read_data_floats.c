/*#######################################_Pier_Luigi_Manfrini_######################################
 *
 *	This is for dbg/manage the solution04
 *
 *	This program receives as input a data file with floats and show the contained numbers 
 *	to STDERR
 *
 *	Usage:
 *		<./read_data_floats> <data_floats>
 *	
 *	Compile:
 *		gcc read_data_floats.c -o read_data_floats -Wall -Werror -fsanitize=leak
 *
 * #################################################################################################
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    dprintf(2, "Wrong Usage: <./read_data_floats> <data_floats>\n");
    exit(EXIT_FAILURE);
  }
  int input;
  if ((input = open(argv[1], O_RDONLY)) == -1)
    perror("Error while opening the input file!\n(PERROR)");
//  size_t filesize = lseek(input, 0, SEEK_END) - lseek(input, 0, SEEK_SET);
//  size_t num_of_floats = filesize / sizeof(float);
  float number;
  dprintf(1, "FLOATS in file %s:\n\n\t", argv[1]);
  while (read(input, &number, sizeof(float)) == sizeof(float)) {
    dprintf(1, "->%f\n\t", number);
  }
  dprintf(1, "\n");
  return 0;
}
