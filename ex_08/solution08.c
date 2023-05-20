/*###############################_Pier_Luigi_Manfrini_################################
 *
 *	This program basically copies a file. It accepts two arguments from CLI which
 *	represent the source file and destination file. At execution two
 *	processes are created: the father reads the file (possibly large for testing
 *	purposes) and pass it through a pipe to the child process which reads it and
 *	writes it to destination.
 *
 *	Usage:
 *		<./solution08> <src> <dest>
 *
 *
 *	Compile as:
 *		gcc solution08.c -o solution08 -Wall -Werror -fsanitize=leak
 *
 * ###################################################################################
 */
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ 0
#define WRITE 1
#define BUFF_SIZE 1024*1024*20 // 20 Mb

int main(int argc, char **argv) {
if(argc!=3){
	dprintf(2,"Wrong Usage: <./solution08> <source> <destination>\n");
	exit(EXIT_FAILURE);
} 

  char *file;
  int p2child[2];
  if (pipe(p2child) == -1)
    perror("pipe");
  pid_t pid = fork();
  switch (pid) {
  case -1:
    perror("");
    exit(EXIT_FAILURE);
    break;
  case 0: { // Child
    size_t read_tot = 0;
    size_t written_tot = 0;
    ssize_t n_read;
    ssize_t n_written;
    int out;
    if ((out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC,
                    S_IRUSR | S_IWUSR)) == -1)
      perror("");
    if (close(p2child[WRITE]) == -1)
      perror("");
    file = (char *)malloc(BUFF_SIZE * sizeof(char));
    int i = 1;

/* Child reads from pipe BUFF_SIZE bytes at a time */
    while ((n_read = read(p2child[READ], file + read_tot, BUFF_SIZE)) > 0) {
     if(n_read==BUFF_SIZE)
	     file = (char *)realloc(file, BUFF_SIZE * (i + 1) * sizeof(char));
      if (file == NULL)
        perror("");
      read_tot += n_read;
      if ((n_written = write(out, file + written_tot, n_read)) != n_read)
        perror("write in child");
      written_tot += n_written;
      i++;
    }
    if (n_read == -1)
      perror("read in child");
    close(p2child[READ]);
    assert(written_tot == read_tot);
    free(file);
    exit(EXIT_SUCCESS);
    break;
  }
  default: { // Father
    if (close(p2child[READ]) == -1)
      perror("");
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
      perror("open");
    size_t filesize = lseek(fd, 0, SEEK_END) - lseek(fd, 0, SEEK_SET);
    file = (char *)malloc(filesize * sizeof(char));
    if(read(fd, file, filesize)!=filesize)
	    perror("");
    if (write(p2child[WRITE], file, filesize) != filesize)
      perror("");
    close(p2child[WRITE]);
    free(file);
    break;
  }
  }
  if (waitpid(pid, 0, 0) == -1)
    perror("");
  return 0;
}
