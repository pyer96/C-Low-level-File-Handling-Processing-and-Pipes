/*##############################_Pier_Luigi_Manfrini_####################################
 *
 *	This program creates N children and generate 100 messages to be sent
 *	randomly to those children. The max length of the messages created is
 *	specified by the user, as well as N. Once all 100 msgs have been sent,
 *	each child process will print what it received, if anything.
 *
 *	Usage:
 *		<./solution12> <N> <max_lenght>
 *
 *	Compile:
 *		gcc solution12.c -o solution12 -Wall -Werror -fsanitize=leak
 *
 * ######################################################################################
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define green "\033[0;32m"
#define yellow "\033[0;33m"
#define RESET "\033[0m"

#define NUM_MSG 100
#define WRITE 1
#define READ 0

void rnd_msg_gen(char *, size_t);
int main(int argc, char **argv) {
  if (argc != 3) {
    dprintf(2, "Wrong Usage: <./a.out> <N> <Max_Length>\n");
    exit(EXIT_FAILURE);
  }
  srand((unsigned int)time(NULL));
  int N = atoi(argv[1]);
  int max_len = atoi(argv[2]);
  if (N <= 0 || max_len <= 0) {
    dprintf(2, "N and Max_Length must be positive integers!\n");
    exit(EXIT_FAILURE);
  }
  dprintf(1,
          "GENERATING %d MESSAGES and %d CHILDREN\nmax string length: %d\t( "
          "\\0 included )\n\n",
          NUM_MSG, N, max_len);
  pid_t *pid = (pid_t *)malloc(N * sizeof(pid_t));
  int **pipes = (int **)malloc(N * sizeof(int *));

  /* Fork N times */

  for (int i = 0; i < N; i++) {
    pipes[i] = (int *)malloc(2 * sizeof(int));
    pipe(pipes[i]);
    switch ((pid[i] = fork())) {
    case -1:
      perror("");
      exit(EXIT_FAILURE);
      break;
    case 0: { // Child
      char *msg[NUM_MSG + 1];
      int received_msg = 0;
      int child_num = i;
      close(pipes[i][WRITE]);
      // Close also all previous file descriptors inerithed open from father
      // which closes only the read end
      for (int j = (i - 1); j >= 0; j--) {
        close(pipes[j][WRITE]);
      }
      while (1) {
        ssize_t msg_length = 0;
        if (read(pipes[i][READ], &msg_length, sizeof(ssize_t)) !=
            sizeof(ssize_t))
          perror("read1_child");
        if (msg_length == -2) {
          break;
        }
        msg[received_msg] = (char *)malloc(msg_length);
        if (read(pipes[i][READ], msg[received_msg], msg_length) != msg_length)
          perror("read2_child");
        received_msg++;
      }
      msg[received_msg] = NULL;
      for (int i = 0; msg[i] != NULL; i++) {
        dprintf(1, "%sCHILD %d%s>>" RESET " Msg %s%d/%s%d%s: \t%s\n",
                (msg[i + 1] == NULL) ? GREEN : green, child_num + 1,
                (child_num + 1 >= 10) ? "" : "\t",
                (i + 1 < 10) ? "  " : (i + 1 < 100) ? " " : "", i + 1,
                (received_msg < 10) ? " " : "", received_msg,
                (received_msg < 100) ? " " : "", msg[i]);
        free(msg[i]);
      }
      close(pipes[child_num][READ]);
      exit(EXIT_SUCCESS);
      break;
    }
    default: { // Father
      close(pipes[i][READ]);
      break;
    }
    } // end switch-case
  }

  /* Create and dispatch 100 rnd messages */
  char *messages[NUM_MSG];
  for (int i = 0; i < NUM_MSG; i++) {
    /* The string length ranges randomly between max_len/2 and max_length */
    ssize_t length = (ssize_t)((max_len + 1) / 2 + rand() % (max_len + 1) / 2);
    messages[i] = (char *)malloc(length * sizeof(char));
    rnd_msg_gen(messages[i], length);
    int random_destination = (int)(rand() % N);
    if (write(pipes[random_destination][WRITE], &length, sizeof(ssize_t)) !=
        sizeof(ssize_t)) // since the length is random is communicated to the
                         // child right before the message body
      perror("wr1_fath");
    if (write(pipes[random_destination][WRITE], messages[i], length) != length)
      perror("wr2_fath");
    dprintf(1,
            RED "FATHER\t>>" RESET " Msg %s%d/%d%s: \t%s " yellow
                "\t~>Sent to child %d" RESET "\n",
            (i + 1 < 10) ? "  " : (i + 1 < 100) ? " " : "", i + 1, NUM_MSG,
            (NUM_MSG < 100) ? " " : "", messages[i], random_destination + 1);
    free(messages[i]);
  }
  ssize_t stop = -2;
  for (int i = 0; i < N; i++) {
    if (write(pipes[i][WRITE], &stop, sizeof(ssize_t)) != sizeof(ssize_t))
      perror("");
    close(pipes[i][WRITE]);
    free(pipes[i]);
    if (waitpid(pid[i], 0, 0) == -1)
      perror("");
  }
  free(pipes);
  free(pid);
  return 0;
}

void rnd_msg_gen(char *msg, size_t length) {
  for (int i = 0; i < length - 1; i++) {
    // readable ascii chars between 33 and 126 (126-33=93)
    msg[i] = (char)(33 + (rand() % 94));
  }
  msg[length - 1] = '\0';
}
