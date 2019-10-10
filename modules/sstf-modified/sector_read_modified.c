
/*
 * Simple disc I/O generator
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFFER_LENGTH 512
#define DISK_SZ	1073741824

int ret, fd, pid;
char buf[BUFFER_LENGTH];

void sectorRead(int flag){

    int i;
    unsigned int pos;

	srand(getpid());

	for (i = 0; i < 10; i++){
        pos = (rand() % (DISK_SZ >> 9));
	    printf("%d | Sector: %lu\n", flag, pos);
		lseek(fd, pos * 512, SEEK_SET);
		read(fd, buf, 100);
    }
}

int main(){


	printf("Starting sector read example...\n");

	printf("Cleaning disk cache...\n");
	system("echo 3 > /proc/sys/vm/drop_caches");

	fd = open("/dev/sdb", O_RDWR);
	if (fd < 0){
		perror("Failed to open the device...");
		return errno;
	}

	strcpy(buf, "hello world!");

    int flag = 0;

    if (fork() == 0){

        sectorRead(0);
    }
    else{

        flag++;
        if (fork() == 0){

            sectorRead(flag);
        }
        else{

            flag++;
            if (fork() == 0){

                sectorRead(flag);
            }
        }
    }
    
    

    //for (i = 0; i < 50; i++){
	//	pos = (rand() % (DISK_SZ >> 9));
	//	printf("Sector: %lu\n", pos);
	//	/* Set position */
	//	lseek(fd, pos * 512, SEEK_SET);
	//	/* Peform read. */
	//	read(fd, buf, 100);
	//}

	close(fd);

	return 0;
}
