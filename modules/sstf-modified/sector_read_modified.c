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

int main(){
	int ret, fd, pid, i;
    //unsigned int posi[5] = {1261960, 689984, 64968, 994448, 1603288};
    unsigned int posi[12] = {1,2,3,4,5,6,7,8,9,10,11,12};
	unsigned int pos, flag, min, max;
	char buf[BUFFER_LENGTH];

	printf("Starting sector read example...\n");

	printf("Cleaning disk cache...\n");
	system("echo 3 > /proc/sys/vm/drop_caches");

	srand(getpid());

	fd = open("/dev/sdb", O_RDWR);
	if (fd < 0){
		perror("Failed to open the device...");
		return errno;
	}

	strcpy(buf, "hello world!");

	for (i = 0; i < 12; i++){

        if (i % 4 == 0){

            if (fork() == 0){

                flag = 1;
                i += 4;
                max = i + 3;
            }
            else{

                max = i + 3;
                flag = 0;
            }
        }

        if (i == max) i = 12;

		printf("Flag: %lu | Sector: %lu\n", flag, posi[i]);

	    lseek(fd, posi[i] * 512, SEEK_SET);
	    read(fd, buf, 100);
	}
	close(fd);

	return 0;
}
