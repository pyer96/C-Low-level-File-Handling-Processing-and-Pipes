/*#################################_Pier_Luigi_Manfrini_################################
 *
 * 	This program receives two text files from CLI and reads from the
 *	first a list of integers, finds which of them are prime numbers and
 *	writes the latter into the second text file provided.
 *
 *	Usage:
 *		<./solution03> <input_file> <output_file>
 *
 *	Note: 	-If the output_file doesn't already exists is created;
 *		if instead already exists is overwritten.
 *	
 *	Compile:
 *		gcc solution03_txt.c -o solution03_txt -Wall -Werror -fsanitize=leak
 *
 *######################################################################################
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>



/*Returns 0 if NOT PRIME, 1 if number is PRIME*/
int check_prime(int n);

int main(int argc, char **argv) {
  if (argc != 3) {
    dprintf(2, "Wrong Usage: <./solution03.c> <input_file> <output_file>\n");
    exit(1);
  }
  int input_file = open(argv[1], O_RDONLY);
  int output_file =
      open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  size_t inputfilesize =
      lseek(input_file, 0, SEEK_END) - lseek(input_file, 0, SEEK_SET);
  char *filebuffer_in = (char *)malloc(inputfilesize + 1);
  if (read(input_file, filebuffer_in, inputfilesize) != inputfilesize)
    perror("Error while reading from input file\n(PERROR)");
  filebuffer_in[inputfilesize] = '\0';
  size_t max_lenght = inputfilesize;
  int* numbers = (int *)malloc(max_lenght*sizeof(int));
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
    }
  }
  char buff[20];
  for (int j = 0; j < i; j++) {
    if (check_prime(numbers[j]) == 1) {
      if (sprintf(buff, "%d\n", numbers[j]) <= 0) {
        perror("Error while composing the string to be written!\n(PERROR)");
      }
      write(output_file, buff, strlen(buff));
      /* I PREVIOUSLY USED TO USE dprintf WHICH SEEMED MORE
       * CONVENIENT TO ME
       *	dprintf(output_file, "%d\n", numbers[j]);
       */
    }
  }
  free(filebuffer_in);
  free(numbers);
  close(input_file);
  close(output_file);
}

int check_prime(int n) {
  int flag = 0;
  if (n == 1) {
    dprintf(2, "%d is prime!\n", n);
    return 1; // I consider 1 to be a PRIME number
  } else if (n <= 0)
    return 0;
  else {
    for (int i = 2; i < n / 2; i++) {
      if (n % i == 0) {
        flag = 1;
        break;
      }
    }
  }
  if (flag == 0) {
    dprintf(2, "%d is prime!\n", n);
    return 1;
  } else
    return 0;
}
