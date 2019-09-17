#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>

#define SYSCALL_PROCESSSLEEPINFO	386

int main(int argc, char** argv)
{  
    char buf[256];
    long ret;

    printf("Invoking 'listProcessSleepInfo' system call.\n");
       
    ret = syscall(SYSCALL_PROCESSSLEEPINFO, buf, sizeof(buf)); 

	/* Success, show the process info. */       
    if(ret > 0) printf("%s\n", buf);
    else printf("System call 'listProcessSleepInfo' did not execute as expected error %d\n", ret);
        
    return 0;
}
