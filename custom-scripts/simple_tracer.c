#include <stdio.h>
#include <stdlib.h>

int main(){
  
  	int traceon_fd = -1;
  	int marker_fd = -1;
  	int tracef_fd = -1;
  	const char *path;
  	struct timespec req;
  
  	/* filter by process pid */
  	set_ftrace_pid();
  
  	/* set current_tracer */
  	set_tracer("function_graph");
  
  	path = find_tracefs();
  
  	if (!path) {
  		printf("tracefs not found.\n");
  		exit(1);
  	}
  
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
