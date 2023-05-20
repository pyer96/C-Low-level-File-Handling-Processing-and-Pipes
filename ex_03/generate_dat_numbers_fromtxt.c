/*#################################################_Pier_Luigi_Manfrini_#########################################
 *
 *	This program is outside the program asked by the laboratory, since it is
 *	for handling the files needed to test the SOLUTION03 executables.
 *
 *	This program receives from CLI a txt file with numbers in it and creates
 *	a binary data file (named "<inputfilename>.dat" with those same numbers
 *
 *	Usage:
 *		<generate_dat_numbers_fromtxt> <numbers>
 *
 *	Compile:
 *		gcc generate_dat_numbers_fromtxt.c -o generate_dat_numbers_fromtxt -Wall -Werror -fsanitize=leak
 *
 *###############################################################################################################
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define NUM 13

int main(int argc, char **argv) {
  int input, output;
  if (argc != 2) {
    dprintf(2, "Wrong Usage: <generate_dat_numbers_fromtxt> <input.txt>\n");
    exit(EXIT_FAILURE);
  }
  if ((input = open(argv[1], O_RDONLY)) == -1)
    perror("Error while opening input file\n(PERROR)");
  char outname[50];
 strcpy(outname,argv[1]);
 strcpy(outname+strlen(argv[1]),".dat");
  if ((output = open(outname, O_RDWR | O_CREAT | O_TRUNC,
                     S_IRUSR | S_IWUSR)) == -1)
    perror("Error while opening output file\n(PERROR)");
  size_t inputfilesize = lseek(input, 0, SEEK_END) - lseek(input, 0, SEEK_SET);
  char *filebuffer_in;
  if ((filebuffer_in = (char *)malloc(inputfilesize + 1)) == NULL)
    perror("Error while allocating memory in the heap for storing the input "
           "text file\n(PERROR)");
  filebuffer_in[inputfilesize] = '\0';
  if (read(input, filebuffer_in, inputfilesize) != inputfilesize)
    perror("Error while reading the input file\n(PERROR)");
 // dprintf(2, "DBG:\ninputfilesize ---->  %lu\nfilebuffer:\n%s\n",
 //         inputfilesize, filebuffer_in);
  size_t max_lenght = inputfilesize;
  int *numbers = (int *)malloc(max_lenght * sizeof(int));
  int i = 0;
  char *token;
  if ((token = strtok(filebuffer_in, " \n")) != NULL) {
    for (i = 0; i < max_lenght; i++) {
      if (i == 0) {
        numbers[i] = atoi(token);
      } else {
        if ((token = strtok(NULL, " \n")) == NULL)
          break;
        numbers[i] = atoi(token);
      }
      if (write(output, &numbers[i], sizeof(int)) != sizeof(int))
        perror("Error while writing in the output file\n(PERROR)");
    }
  }
  free(filebuffer_in);
  free(numbers);
  close(input);
  close(output);
}
