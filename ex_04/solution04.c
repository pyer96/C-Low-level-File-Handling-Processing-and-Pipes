/*##########################_Pier_Luigi_Manfrini_###############################
 *
 *	This program copies floating point numbers from an input file into
 *	an output file. The output file, if no specifying option is provided,
 *	will be of the same format of the input file.
 *	(output file, if already existing, is overwritten!)
 *
 *	Usage:
 *		<solution04> <input_file> <output_file> [OPTION]
 *
 *	Note:
 *
 *	-Available OPTIONS's flags:
 *		-b	-> binary data output
 *		-t 	-> text file output
 *		-x 	-> hexadecimal output
 *
 *	-Additionally --help flag is implemented:
 *		<./solution04> --help
 *	 It will yield the correct usage as well as the available options
 *	
 *	Compile:
 *		gcc solution04.c -o solution04 -Wall -Werror -fsanitize=leak
 *
 *##############################################################################
 */

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static bool OPTION_PROVIDED = false;

float **extract_numbers(char *);
float **read_from_data(int);
char *read_from_text(int);
void write_data(int, float **);
void write_txt(int, float **);
void write_hex(int, float **);

int main(int argc, char **argv) {
  if (argc == 2 && (strcmp(argv[1], "--help") == 0)) {
    dprintf(
        2,
        "Program Usage:\n\n\t<./solution04> <input_file> <output_file> [OPTION]\n\n\n\
Available options:\n\n\
\t-b : forces the output file to be binary\n\
\t-t : forces the output file to be a text one\n\
\t-x : forces the output file to be a hexadecimal representation\n\n\
If no OPTION is provided the output file is by default of the\n\
same format of the input!\n");
    return 0;
  }

  if (argc < 3) {
    dprintf(
        2,
        "Wrong Usage: <./solution04> <input_file> <output_file> [-b -t -x]\n");
    exit(EXIT_FAILURE);
  }
  if (argc == 4)
    OPTION_PROVIDED = true;

  int input, output;
  float **numbers = NULL;
  char *txt = NULL;
  if ((input = open(argv[1], O_RDONLY)) == -1)
    perror("Error while opening the input file\n(PERROR)");
  if ((output = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC,
                     S_IRUSR | S_IWUSR)) == -1)
    perror("Error while opening the output file\n(PERROR)");

  printf(
      "Specify the FORMAT of the INPUT file:\n1)\tBINARY\n2)\tTEXT\nSelect one "
      "of the 2 formats: ");
  int select;
  scanf("%d", &select);
  switch (select) {
  case 1: { // if input is BINARY
    numbers = read_from_data(input);
    if (!OPTION_PROVIDED) {
      write_data(output, numbers);
      for (int i = 0; numbers[i] != NULL; i++) {
        free(numbers[i]);
      }
      free(numbers);
      return 0;
    }
    break;
  }
  case 2: { // if input is TEXTUAL
    txt = read_from_text(input);
    numbers = extract_numbers(txt);
    if (!OPTION_PROVIDED) {
      write_txt(output, numbers);
      int i;
      for (i = 0; numbers[i] != NULL; i++) {
        free(numbers[i]);
      }
      free(numbers);
      free(txt);
      return 0;
    }
    break;
  }
  default: {
    dprintf(2, "Input a valid choice!\n");
    exit(EXIT_FAILURE);
    break;
  }
  }
  /* At this point, after extract_numbers, i am sure to have the float** numbers
   * variable correctly filled up. Code below is executed only if an option
   * has been provided.
   */

  if (strcmp(argv[3], "-b") == 0) {
    write_data(output, numbers);
  } else if (strcmp(argv[3], "-t") == 0) {
    write_txt(output, numbers);
  } else if (strcmp(argv[3], "-x") == 0) {
    write_hex(output, numbers);
  }
  close(input);
  close(output);
  free(txt);
  int t;
  for (t = 0; numbers[t] != NULL; t++) {
    free(numbers[t]);
  }
  free(numbers);
  return 0;
}

void write_data(int output, float **numbers) {
  for (int i = 0; numbers[i] != NULL; i++) {
    if (write(output, numbers[i], sizeof(float)) != sizeof(float))
      perror("Error while writing the output file!\n(PERROR)");
  }
}

void write_txt(int output, float **numbers) {
  char buff[20];
  /* Numbers (array of pointers to float)  is constructed
   * as to have the first positions filled with pointers
   * to the floats just read and subsequently one pointer to NULL
   */
  for (int i = 0; numbers[i] != NULL; i++) {
    if (sprintf(buff, "%f\n", *numbers[i]) < 0)
      dprintf(2, "Error while converting to txt!\n");

    if (write(output, buff, strlen(buff)) != strlen(buff))
      perror("Error while writing the output file\n(PERROR)");
  }
}

void write_hex(int output, float **numbers) {
  char buff[20];
  for (int i = 0; numbers[i] != NULL; i++) {
    if (sprintf(buff, "%a\n", *numbers[i]) < 0)
      dprintf(2, "Error while converting to hex!\n");
    if (write(output, buff, strlen(buff)) != strlen(buff))
      perror("Error while writing the output file\n(PERROR)");
  }
}

float **read_from_data(int input) {
  float **numbers;
  size_t filesize = lseek(input, 0, SEEK_END) - lseek(input, 0, SEEK_SET);
  size_t num_of_floats = filesize / sizeof(float);
  numbers = (float **)malloc(num_of_floats * sizeof(float *) + sizeof(float *));
  for (int i = 0; i < num_of_floats; i++) {
    numbers[i] = (float *)malloc(sizeof(float));
  }
  numbers[num_of_floats] = NULL;
  for (int i = 0; i < num_of_floats; i++) {
    if (read(input, numbers[i], sizeof(float)) != sizeof(float))
      perror("Error while reading the input file\n(PERROR)");
  }
  return numbers;
}

char *read_from_text(int input) {
  char *txt;
  size_t filesize = lseek(input, 0, SEEK_END) - lseek(input, 0, SEEK_SET);
  txt = (char *)malloc(filesize + sizeof(char));
  txt[filesize] = '\0';
  if (read(input, txt, filesize) != filesize)
    perror("Error while reading the input file\n(PERROR)");
  return txt;
}

float **extract_numbers(char *txt) {
  /* In order to estimate a maximum number of float* to allocate for,
   * we consider at most 1 float every 2 chars (which is a large upper bound)
   * 	# chars = strlen(txt) (txt is securely set to end with '\0')
   * 	max # floats = # chars / 2
   */
  float **numbers;
  size_t max_length = strlen(txt);
  numbers = (float **)malloc((max_length / 2) * sizeof(float *));
  char *tok = strtok(txt, " \n");
  numbers[0] = (float *)malloc(sizeof(float));
  if (sscanf(tok, "%f", &numbers[0][0]) != 1)
    perror("Error while parsing the input text floats\n(PERROR)");
  for (int i = 1; i < max_length; i++) {
    if ((tok = strtok(NULL, " \n")) == NULL) {
      numbers[i] = NULL;
      break;
    }
    numbers[i] = (float *)malloc(sizeof(float));
    if (sscanf(tok, "%f", &numbers[i][0]) != 1)
      perror("Error while parsing the input text floats\n(PERROR)");
  }
  return numbers;
}
