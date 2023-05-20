/*##########################_Pier_Luigi_Manfrini#########################
 *
 *	Manage low level files:
 *
 *	This program generates a 3x3 matrix of random floats
 *	between 0 and 50.0 (inclusive)
 *	and saves it on a file defined through CLI argument.
 *	The file must already exists!
 *	Usage:
 *		<./solution02> <filename>
 *
 *	Compile:
 *		gcc solution02.c -o solution02 -Wall -Werror
 *
 *#######################################################################
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MATRIX_SIZE 3
#define GREEN "\033[1;32m"
#define RESET "\033[0m"

void print_matrix(float matrix[MATRIX_SIZE][MATRIX_SIZE]);

int main(int argc, char **argv) {
  if (argc != 2) {
    dprintf(2, "Wrong Usage: <./solution02> <filename> \n");
    return 1;
  }
  int fd = open(argv[1], O_RDWR | O_TRUNC);
  //  FILE* file = fdopen(fd, "w");
  if (fd == -1) {
    perror("Failed to open the specified file\n(PERROR)");
    return 1;
  }
  /*
   *	Since we hard-code in advance the matrix's
   *	 size we predefine the matrix in the stack
   */
  float matrix[MATRIX_SIZE][MATRIX_SIZE];
  /*
   *	Initialize the seed for CSRNG rand() and set range in [0 MAX]
   *	(rand() produces pseudo-random numbers in the range [0 RAND_MAX])
   */
  srand((unsigned int)time(NULL));
  float max = 50.0;
  for (int row = 0; row < MATRIX_SIZE; row++) {
    for (int col = 0; col < MATRIX_SIZE; col++) {
      matrix[row][col] = ((float)rand() / (float)(RAND_MAX)) * max;
    }
  }
  /*	DBG
   *
   *	print_matrix(matrix);
   */
  printf(GREEN"GENERATED MATRIX"RESET"\n");
  print_matrix(matrix);

  write(fd, matrix, sizeof(matrix));

  /*	WE READ THE MATRIX JUST WRITTEN IN THE FILE TO CHECK THAT IS
   *	WHAT WE EXPECT
   */
  lseek(fd, 0, SEEK_SET); // reset the iterator inside the file descriptor
  float retrieved[MATRIX_SIZE][MATRIX_SIZE];
  read(fd, retrieved, sizeof(matrix));
  printf(GREEN"\n\nRETRIEVED (from file) MATRIX"RESET"\n");
  print_matrix(retrieved);

  close(fd);
}

void print_matrix(float mat[MATRIX_SIZE][MATRIX_SIZE]) {
  for (int row = 0; row < MATRIX_SIZE; row++) {
    for (int col = 0; col < MATRIX_SIZE; col++) {
      dprintf(1, "%f ", mat[row][col]);
    }
    dprintf(1, "\n");
  }
}
