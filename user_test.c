#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
 
#define DEVICE_NODE "/dev/vchar_dev"
#define BUFFER_LENGTH 4   	         // The buffer length 
static char receive[BUFFER_LENGTH];      // The receive buffer from the LKM
 
int main() {
	int ret, fd;

	fd = open("/dev/vchar_dev", O_RDWR);             
	if (fd < 0) {
		perror("Failed to open the device...\n");
		return errno;
	}

	printf("Reading from the device...\n");
	ret = read(fd, receive, BUFFER_LENGTH);         // Read the response from the module
	if (ret < 0) {
		printf("Failed to read the device...\n");
		return errno;
	}
	else {
		printf("Random number is (%s)\n", receive);
	}
	close(fd);
	return 0;
}

