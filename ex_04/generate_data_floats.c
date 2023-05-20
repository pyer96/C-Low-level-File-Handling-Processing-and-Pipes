/*#######################################_Pier_Luigi_Manfrini_######################################
 *
 *	This is for dbg/manage the solution04
 *
 *	This program receives as input a txt file with floats and stores them
 *	in an output data file, named "data_floats". If output file already exists
 *	is overwritten
 *
 *	Usage:
 *		<./generate_data_floats> <floats_txt>
 *	
 *	Compile:
 *		gcc generate_data_floats.c -o generate_data_floats -Wall -Werror -fsanitize=leak
 *
 * #################################################################################################
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    dprintf(2, "Wrong Usage: <./generate_data_floats> <float_txt> \n");
    exit(EXIT_FAILURE);
  }
  int input, output;
  if ((input = open(argv[1], O_RDONLY)) == -1)
    perror("Error while opening the input file!\n(PERROR)");
  if ((output = open("data_floats", O_WRONLY | O_CREAT | O_TRUNC,
                     S_IRUSR | S_IWUSR)) == -1)
    perror("Error while opening the input file!\n(PERROR)");

  size_t filesize = lseek(input, 0, SEEK_END) - lseek(input, 0, SEEK_SET);
  char *buff = (char *)malloc(filesize + 1);
  buff[filesize] = '\0';
  if (read(input, buff, filesize) != filesize)
    perror("Error while reading the input file!\n(PERROR)");
  float *numbers = (float *)malloc((filesize / 2) * sizeof(float));
  char *tok = strtok(buff, " \n");
  if (sscanf(tok, "%f", &numbers[0]) != 1)
    perror("Error while parsing the txt float!\n(PERROR)");
  dprintf(2, "FLOATS RETRIEVED:\n\t-> %f\n\t", numbers[0]);

  if (write(output, &numbers[0], sizeof(float)) != sizeof(float))
    perror("Error while writing the output file!\n(PERROR)");

  for (int i = 1; (tok = strtok(NULL, " \n")) != NULL; i++) {
    if (sscanf(tok, "%f", &numbers[i]) != 1)
      perror("Error while parsing the txt float\n(PERROR)");
    dprintf(2, "-> %f\n\t", numbers[i]);
    if (write(output, &numbers[i], sizeof(float)) != sizeof(float))
      perror("Error while writing the output file!\n(PERROR)");
  }
  dprintf(2, "\n");
  free(buff);
  free(numbers);
  return 0;
}
