/*################################_Pier_Luigi_Manfrini_##############################
 *	
 *	This program receives two data files from CLI and reads from the
 *	first a list of integers, finds which of them are prime numbers and
 *	writes the latter into the second text file provided.
 *
 *	Usage:
 *		<./solution03_dat> <input_file> <output_file>
 *
 *	Note: 	-If the output_file doesn't already exists is created;
 *		if instead already exists is overwritten.
 *	
 *	Compile:
 *		gcc solution03_dat.c -o solution03_dat -Wall -Werror -fsanitize=leak
 *
 *###################################################################################
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


/*Returns 0 if NOT PRIME, 1 if number is PRIME*/
int check_prime(int n);

int main(int argc, char **argv) {
  if (argc != 3) {
    dprintf(2, "Wrong Usage: <./solution03_dat> <input_file> <output_file>\n");
    exit(1);
  }
  int input_file = open(argv[1], O_RDONLY);
  int output_file =
      open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  
  size_t filesize = lseek(input_file,0,SEEK_END) -lseek(input_file,0,SEEK_SET);
  size_t length = filesize/sizeof(int);
  int* numbers = (int*)malloc(filesize);
  int i = 0;
  while (i< length && read(input_file,&numbers[i],sizeof(int)) > 0) {
    i++;
  }
  for (int j = 0; j < i; j++) {
    if (check_prime(numbers[j]) == 1) {
      write(output_file,&numbers[j], sizeof(int));
    }
  }
  free(numbers);
  close(input_file);
  close(output_file);
}

int check_prime(int n) {
  int flag = 0;
  if ((n == 1) | (n == 2)) {
    dprintf(2, "%d is prime!\n", n);
    return 1; // I consider 1 and 2 to be PRIME numbers
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
