#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	int pid;
	/* fork a child process */
	pid = fork();
	/* check fork() return code */
	if ( pid < 0 ) {
		/* some error occurred */
		fprintf( stderr, "Fork failed!\n" );
		exit( -1 );
	} else if ( pid == 0 ) {
		/* this is the child process */
		execl( "/bin/ls", "ls", NULL ); /* morph into “ls” */
	} else {
		/* this is the parent process. Wait for child to complete */
		wait( NULL );
		printf( "Child completed -- parent now exiting.\n" );
		exit( 0 );
	}
}

