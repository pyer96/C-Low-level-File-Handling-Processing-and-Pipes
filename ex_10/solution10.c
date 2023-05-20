/*###############################_Pier_Luigi_Manfrini_####################################
 *
 *	This is a simple Producer-Consumer program in which the queue between
 *      the two processes (Father and Son) is at most 4 tokens.
 *      Two pipes are implemented as semaphores between the two processes and
 *      are initialized to grant the upbound of at most 4 units. While in
 *	"critical section" the producer (child process) writes a file in the
 *	current working directory and the consumer (father process) deletes it.
 *
 *	Usage:
 *		<./solution10>
 *
 * 	Notes:
 * 		-DEFINES provided to manage and change some settings, in particular: 
 * 			> TOKEN_NUMBER: can be used to set the max "queue" lenght of the
 * 					"semaphore"
 * 			> SLOW_PRODUCER: 0 for false and 1 for active flag, with a slow 
 * 					producer there won't be, while the program is 
 * 					running, any "enqueued" files in the CWD 
 * 			> SLOW_CONSUMER: 0 for false and 1 for active flag, with a slow 
 * 					consumer there will be, while the program is 
 * 					running, 4 "enqueued" files in the CWD
 *
 *	Compile:
 *		gcc solution10.c -o solution10 -Wall -Werror -fsanitize=leak
 *
 * #######################################################################################
 */

#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

#define TOKEN_NUMBER 4
#define FILENAME_LENGTH 6
#define SLOW_PRODUCER 1 // either 0(false) or 1(true)
#define SLOW_CONSUMER 0 // either 0(false) or 1(true)

void generate_random_filename();
void handler(int signum);
static char slot = 's';

int main() {

  pid_t pid;
  bool active = true;

  /* Create and Initialize two pipes for the two semaphores */
  int sem_slot[2];
  int sem_token[2];
  if (pipe(sem_slot))
    perror("slot pipe");
  if (pipe(sem_token))
    perror("token pipe");

  /* Push 4 slots available for the producer into the sem_slot pipe */
  for (int i = 0; i < TOKEN_NUMBER; i++) {
    if (write(sem_slot[WRITE], &slot, 1) != 1)
      perror("write slot");
  }

  /* sigaction structs for handling SIGINT */
  struct sigaction new, old;
  new.sa_handler = handler;
  new.sa_flags = SA_RESTART;
  sigaction(SIGINT, NULL, &old);
  if (old.sa_handler != SIG_IGN) // we don't want to change behaviour to a
                                 // signal which was on ignore (SIG_IGN)
    sigaction(SIGINT, &new, &old);
  /* Initialize seed for rand() */
  srand((unsigned int)time(NULL));

  /* Fork into Producer and Consumer */
  if ((pid = fork()) == -1) { // Fork Failure
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) { // Child ~ Producer

    /* set SIG_IGN as SIGINT handler in the child */
    struct sigaction set;
    set.sa_handler = SIG_IGN;
    set.sa_flags = SA_RESTART;
    sigaction(SIGINT, &set, NULL);

    close(sem_token[READ]);
    close(sem_slot[WRITE]);
    char filename[FILENAME_LENGTH];
    static int fd;

    /* At beginning 4 slots are already available */

    /* PRODUCER LOOP */
    while (active) {
      if (SLOW_PRODUCER)
        sleep(1);
      char read_slot;
      generate_random_filename(filename);
      if (read(sem_slot[READ], &read_slot, 1) != 1) // wait for available slots
        perror("");
      if (read_slot == '\0') {
        active = false;
        filename[0] = '\0';
        dprintf(2, "child@PRODUCER>>\t\tNull byte slot received: Stopped "
                   "producing and writing a null token "
                   "and exiting...\n");
        if (write(sem_token[WRITE], filename, FILENAME_LENGTH) !=
            FILENAME_LENGTH)
          perror("write token");
        break;
      }
      if ((fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY,
                     S_IRUSR | S_IWUSR)) == -1)
        perror("");
      dprintf(
          2,
          "\033[1;32mPRODUCER (child) created the regular file:\t%s\n\033[0m",
          filename);
      close(fd);
      if (write(sem_token[WRITE], filename, FILENAME_LENGTH) !=
          FILENAME_LENGTH) // "signal" a new available token
        perror("write token");
    } // End Loop

    close(sem_slot[READ]);
    close(sem_token[WRITE]);
    exit(EXIT_SUCCESS);
  } // Child End

  else if (pid > 0) { // Father ~ Consumer
    close(sem_token[WRITE]);
    // close(sem_slot[READ]); We DONT close the read end of this pipe here,
    // otherwise once the child (producer) dies, the writing on the write end of
    // a pipe with no more read ends left opened would be no more possible
    char filename[FILENAME_LENGTH];
    /* CONSUMER LOOP */
    while (active) {
      if (read(sem_token[READ], filename, FILENAME_LENGTH) != FILENAME_LENGTH)
        perror("");
      if (SLOW_CONSUMER)
        sleep(1);
      if (filename[0] == '\0') {
        active = false;
        dprintf(2, "father@CONSUMER>>\t\tNull token received: quitting...\n");
        break;
      } else {
        if (remove(filename) == -1)
          perror("");
        dprintf(2,
                "\033[1;31mCONSUMER (father) deleted the regular "
                "file:\t%s\n\033[0m",
                filename);
        if (write(sem_slot[WRITE], &slot, 1) == -1)
          perror("write slot");
      }
    } // End Loop
  }
  close(sem_slot[READ]);
  close(sem_token[READ]);
  close(sem_slot[WRITE]);
  if (waitpid(pid, 0, 0) == -1)
    perror("waitpid");
  return 0; // Father Ends
}

void generate_random_filename(char *filename) {
  // 122 - 97 = 25 the range of lowercase alphanumeric ascii characters
  filename[FILENAME_LENGTH - 1] = '\0';
  for (int i = 0; i < FILENAME_LENGTH - 1; i++) {
    filename[i] = (char)(97 + (int)(rand() % 26));
  }
}

/* Handler function when SIGINT catched */
void handler(int signum) {
  /* Reset Default Behaviour for SIGINT */
  struct sigaction dflt;
  dflt.sa_handler = SIG_DFL;
  sigaction(signum, &dflt, NULL);
  // the consumer signal a terminating slot that will be read at most 4
  // iteration later from the Producer that will stop!
  slot = '\0';
  dprintf(2, "\nfather@CONSUMER>>\t\tSlot set to null byte\n");
}
