
/*
 * Simple disc I/O generator
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LENGTH 512
#define DISK_SZ	1073741824

int main(){
	int ret, fd, pid, i;
    int lim, factor, flag;
	unsigned int pos;
	char buf[BUFFER_LENGTH];

    // Cria 5 forks
    for (i = 0; i < 4; i++) fork();

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

    // Faz parent e child(ren) esperarem
    usleep(2000000);

    // Faz leitura de setores aleatórios
	for (i = 0; i < 10; i++){
		pos = (rand() % (DISK_SZ >> 9));
		//printf("Sector: %lu\n", pos);
		/* Set position */
		lseek(fd, pos * 512, SEEK_SET);
		/* Peform read. */
		read(fd, buf, 100);
	}

	close(fd);

	return 0;
}
