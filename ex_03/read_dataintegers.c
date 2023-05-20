/*###################################_Pier_Luigi_Manfrini_#################################
 *
 *	This program has been made for development purposes only.
 *
 *	It reads integer values from binary data file provided
 *	by CLI and print them in the STDERR
 *
 *	Usage:
 *		<./read_dataintegers> <numbers.dat>
 *	
 *	Compile:
 *		gcc read_dataintegers.c -o read_dataintegers -Wall -Werror -fsanitize=leak	
 *
 * ########################################################################################
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    dprintf(2, "Wrong Usage: <read_datafiles> <datafile>\n");
    return 1;
  }
  int input;
  if ((input = open(argv[1], O_RDONLY)) == -1)
    perror("Error while opening data file\n(PERROR);");
  /* Here fine way to get filesize and resetting the fd pointer to the at the
   * beginning
   */
  ssize_t filesize = lseek(input, 0, SEEK_END) - lseek(input, 0, SEEK_SET);
  int* numbers= (int*)malloc(filesize);
  int i = 0;
  while (i < filesize / sizeof(int) &&
         read(input, &numbers[i], sizeof(int)) > 0) {
    dprintf(2, "%d\n", numbers[i]);
    i++;
  }
  free(numbers);
  close(input);
  return 0;
}
