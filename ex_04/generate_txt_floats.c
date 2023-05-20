/*####################################_Pier_Luigi_Manfrini_####################################
 *
 *	This is for dbg/manage the solution04
 *
 *	This program generates HOW_MANY (default 20) random floats, ranging
 *	between 0 and RANGE (default 50.0), and writes them
 *	in the float.txt file, which is created if not already existing or
 *	oveerwritten otherwise
 *
 *	Usage:
 *		<./generate_txt_floats>
 *	
 *	Compile:
 *		gcc generate_txt_floats.c -o generate_txt_floats -Wall -Werror -fsanitize=leak
 *
 * ############################################################################################
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define HOW_MANY 20
#define RANGE 50.0

int main(int argc, char **argv) {
  if (argc > 2) {
    dprintf(
        2,
        "Wrong Usage: <./generate_txt_floats> [ OPTIONALLY= <output_name> ]\n");
    exit(EXIT_SUCCESS);
  }
  char *name;
  if (argc == 2)
    name = argv[1];

  else
    name = "floats.txt\0";

  int output;
  if ((output = open(name, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR)) ==
      -1)
    perror("Error while creating the output file!\n(PERROR)");
  srand((unsigned int)time(NULL));
  float numbers[HOW_MANY];
  for (int i = 0; i < HOW_MANY; i++) {
    numbers[i] = ((float)rand() / (float)(RAND_MAX)) * (float)RANGE;
    char buff[20];
    sprintf(buff, "%f\n", numbers[i]);
    if (write(output, buff, strlen(buff)) != strlen(buff))
      perror("Error while writing the output file!\n(PERROR)");
    
  }
 return 0;
}
