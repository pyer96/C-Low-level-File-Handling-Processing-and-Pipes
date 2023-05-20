/*###########################_Pier_Luigi_Manfrini_##############################
 *
 *	This program forks itself one time. The child prints a message to 
 *	STDERR and then exits. Once the father has waited for its child, 
 *	it prints its message comprising of the child's exit status.
 *
 *	Usage:
 *		<./solution06> 
 *	
 *	Compile:
 *		gcc solution06.c -o solution06 -Wall -Werror 	
 *
 * #############################################################################
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main () {
pid_t pid = fork();
int status;
if (pid==-1){
	perror("Error while forking the process!\n(PERROR)"); 
	exit(EXIT_FAILURE);
}
else if (pid==0){
// Child Process
dprintf(2, "Hi!, I (%d) am the child of %d!\n", getpid(),getppid());
exit(EXIT_SUCCESS);
}
else if (pid>0){ 
// Parent Process
waitpid(-1, &status, 0);
dprintf(2,"Hi!, I (%d) am the father of %d! It just exited with status: %d\n", getpid(),pid,status);
}
return 0;
}
