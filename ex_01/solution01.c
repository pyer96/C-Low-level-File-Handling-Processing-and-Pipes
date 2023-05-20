/*##############################_Pier_Luigi_Manfrini_#################################
 *
 *	This program searches for a file in a specific folder.
 *	Both folder and filename are provided by CLI
 *
 *	Usage:
 *		<./solution01> <dir_path> <file_name>
 *	
 *	
 *	Compile:
 *		gcc solution01.c -o solution01 -Wall -Werror
 *
 * ###################################################################################
 */

#include <alloca.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    dprintf(2, "Wrong Usage: <./a.out> <path> <name>\n");
    exit(EXIT_FAILURE);
  }

  DIR *dir_t = opendir(argv[1]);
  if (dir_t == NULL) {
    perror("Error while opening the directory\n(PERROR)");
    exit(EXIT_FAILURE);
  }
  char *filename = argv[2];
  struct dirent *entry;
  int flag = 0;
  while ((entry = readdir(dir_t))) {
    if (entry->d_type == DT_REG &&
        strncmp(entry->d_name, filename, strlen(filename)) == 0) {
      flag = 1;
      dprintf(
          1,
          "The file you looked for is PRESENT in the specified directory!\n");
    }
  }
  if (flag == 0)
    dprintf(
        1,
        "The file you looked for is NOT present in the specified directory\n");
  closedir(dir_t);
  return 0;

  /* OTHER APPROACH USING access()
   *char *arg1 = (char *)malloc(sizeof(argv[1]));
   *strcpy(arg1, argv[1]);
   *char *arg2 = (char *)malloc(sizeof(argv[2]));
   *strcpy(arg2, argv[2]);
   *strcat(arg1, "/");
   *
   * const char *path = strcat(arg1, arg2);
   *int check = access(path, F_OK);
   *if (check == 0)
   * dprintf(1, "The file you asked for exists in the specified directory!\n");
   * else
   *  perror("File not existing or error occurred.\n(PERROR)");
   */
}
