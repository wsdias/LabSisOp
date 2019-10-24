#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>


#define MAX_PATH 256
#define _STR(x) #x
#define STR(x) _STR(x)

static const char *find_tracefs(void){
	static char tracefs[MAX_PATH+1];
	static int tracefs_found = 0;
	char type[100];
	FILE *fp;

	if (tracefs_found)
		return tracefs;

	if ((fp = fopen("/proc/mounts","r")) == NULL)
		return NULL;

	while (fscanf(fp, "%*s %"STR(MAX_PATH)"s %99s %*s %*d %*d\n", tracefs, type) == 2) {
		if (strcmp(type, "tracefs") == 0)
			    break;
	}

	fclose(fp);

	if (strcmp(type, "tracefs") != 0)
		return NULL;

	tracefs_found = 1;

	return tracefs;
}


int open_tracer_file(const char *p, char *file){
	char path[256];
	int fd;

	strcpy(path, p);  /* BEWARE buffer overflow */
	strcat(path, file);
	printf("opening %s\n", path);
	fd = open(path, O_WRONLY);
	if (fd < 0){
		perror("open():");
		exit(fd);
	}

	return fd;
}

void set_ftrace_pid(){
	int fd;
	int pid;
	char strpid[32];
	const char *path = find_tracefs();

	fd = open_tracer_file(path, "/set_ftrace_pid");

	pid = (int)getpid();

	sprintf(strpid, "%d", pid);

	write(fd, strpid, strlen(strpid));

	close(fd);
}

void set_tracer(char *t){
	int fd;
	const char *path = find_tracefs();

	fd = open_tracer_file(path, "/current_tracer");

	write(fd, t, strlen(t));

	close(fd);
}


int main(){

	int traceon_fd = -1;
	int marker_fd = -1;
	int tracef_fd = -1;
	const char *path;
	struct timespec req;

	path = find_tracefs();

	if (!path) {
		printf("tracefs not found.\n");
		exit(1);
	}

	/* filter by process pid */
	set_ftrace_pid();

	/* set current_tracer */
	set_tracer("function_graph");

	/* open tracing_on file */
	traceon_fd = open_tracer_file(path, "/tracing_on");

	/* stop tracer */
	write(traceon_fd, "0", 1);

	/* open tracing_marker file */
	marker_fd = open_tracer_file(path, "/trace_marker");

	/* open trace file */
	tracef_fd = open_tracer_file(path, "/trace");

	/* clean trace log */
	write(tracef_fd, 0, 1);

	req.tv_sec = 0;
	req.tv_nsec = 1000;

	/* start tracer */
	write(traceon_fd, "1", 1);

	write(marker_fd, "before nano\n", 12);
	nanosleep(&req, NULL);
	write(marker_fd, "after nano\n", 11);

	/* stop tracer */
	write(traceon_fd, "0", 1);

	close(traceon_fd);
	close(marker_fd);
	close(tracef_fd);

	return 0;
}




