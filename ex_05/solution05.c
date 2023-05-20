/*##########################_Pier_Luigi_Manfrini_############################
 *
 *	This program creates a random number of child processes.
 *	Each child process creates another random number of
 *	processes, which will sleep for a random number of seconds
 *	The ranges for the PRNG (rand()) are defined as
 *	RANGE_PARENT (default 10), RANGE_CHILDREN (default 5) and
 *	RANGE_SLEEP (default 10).
 *
 *	Usage:
 *		<./solution05>
 *
 *	Compile:
 *		gcc solution05.c -o solution05 -Wall -Werror -fsanitize=leak
 *
 * ##########################################################################
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define RANGE_PARENT 10
#define RANGE_CHILDREN 5
#define RANGE_SLEEP 10

void generate_subchildren();

int main(int argc, char **argv) {
  srand((unsigned int)time(NULL)); // we seed only once!
  int random_num_children = rand() % RANGE_PARENT;
  pid_t *cpid = (pid_t *)malloc(random_num_children * sizeof(pid_t));
  dprintf(2,
          "FATHER ->\t\t[ PID: (%d) ]\tgenerating %d "
          "First Grade Child(ren)\n\n",
          getpid(), random_num_children);
  for (int i = 0; i < random_num_children; i++) {
    if ((cpid[i] = fork()) == -1) {
      perror("Error while forking the parent process!\n(PERROR)");
      exit(EXIT_FAILURE);
      break; // not really necessary
    } else if (cpid[i] == 0) {
      // Child Process
      generate_subchildren();
      exit(1);
      break;
    } else if (cpid[i] > 0) {
    }
  }
  while (wait(NULL) > 0)
    ;
  free(cpid);
  return 0;
}

void generate_subchildren() {
  pid_t holder;
  int random_num_children = rand() % RANGE_CHILDREN;
  pid_t *ccpid = (pid_t *)malloc(random_num_children * sizeof(pid_t));
  dprintf(2,
          "First Grade Child\t[ PID: (%d) ]\tgenerating %d Second Grade "
          "Child(ren)\n",
          getpid(), random_num_children);
  for (int i = 0; i < random_num_children; i++) {
    if ((ccpid[i] = fork()) == -1)
      perror("Error while forking on of the children processes!\n(PERROR)");
    else if (ccpid[i] == 0) {
      // Sub Child process
      unsigned int random_time = (unsigned int)(rand() % RANGE_SLEEP);
      dprintf(2, "Second Grade Child\t[ PID: (%d) ]\tsleeping for %d seconds\n",
              getpid(), random_time);
      sleep(random_time);
      exit(1);
      break; // not really necessary
    } else if (ccpid[i] > 0) {
      // Relative Father
    }
  }
  dprintf(2,
          "First Grade Child\t[ PID: (%d) ]\twaiting for all my %d Children "
          "to finish sleeping\n",
          getpid(), random_num_children);
  int i = 1;
  while ((holder = wait(NULL)) > 0) {
    dprintf(2, "->%d finished sleeping \t[ PID: (%d) ]\n", i++, holder);
  }
  free(ccpid);
}
