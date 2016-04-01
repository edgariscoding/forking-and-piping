// =====================================================================
// Name:		Edgar Sanchez
// Course:		CSCE 3600
// Date:		March 28, 2016
// Title:		Minor Assignment 6
// Version:		1.0
// Description:		Implement a command line pipe program that launches
// 			two applications given as command line arguments using
//			execvp() so that output is piped from one to the other.
// Format:		minor6 command command
// =====================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ  0
#define WRITE 1

int main(int argc, char* argv[]) {
	// Initializes file descriptor and two process ID variables
	int fd[2], pid1, pid2;

	// Verifies number of arguments provided
	if (argc != 3) {
		fprintf(stderr, "Error: Incorrect number of arguments: %i\n", (argc - 1));
		return EXIT_FAILURE;
	}
	else {
		printf("Executing: %s | %s\n", argv[1], argv[2]);
	}

	// Forks first process and runs a command line argument
	if (pipe(fd) == -1) {                       // Creates pipe and checks for errors
		fprintf(stderr, "Error: Failed to create pipe\n");
		return EXIT_FAILURE;
	}
	if ((pid1 = fork()) == 0) {                 // Creates child by calling fork()
		char* cmd1[] = {argv[1], NULL};         // Creates command from argv[1] and a 0
		dup2(fd[WRITE], WRITE);                 // Creates new fd, assigns number, closes old
		close(fd[READ]);
		close(fd[WRITE]);
		execvp(cmd1[0], cmd1);                  // Terminates child if successful
		fprintf(stderr, "Error: Failed to execute first command\n");
		_exit(EXIT_FAILURE);
	}
	else if (pid1 < 0) {                        // Encountered error while forking
		fprintf(stderr, "Error: Failed to fork first child process\n");
		_exit(EXIT_FAILURE);
	}

	// Forks second process and runs a command line argument
	if ((pid2 = fork()) == 0) {                 // Creates child by calling fork()
		char* cmd2[] = {argv[2], NULL};         // Creates command from argv[2] and a 0
		dup2(fd[READ], READ);                   // Creates new fd, assigns number, closes old
		close(fd[WRITE]);
		close(fd[READ]);
		execvp(cmd2[0], cmd2);                  // Terminates child if successful
		fprintf(stderr, "Error: Failed to execute second command\n");
		_exit(EXIT_FAILURE);
	}
	else if (pid2 < 0) {                        // Encountered error while forking
		fprintf(stderr, "Error: Failed to fork second child process\n");
		_exit(EXIT_FAILURE);
	}

	close(fd[WRITE]);                           // Closes parent's write fd

	// Waits for each child to terminate
	for(int i = 0; i < 2; ++i) {
		wait(NULL);
	}

	printf("Command line pipe completed\n");
	return EXIT_SUCCESS;
}