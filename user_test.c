#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
 
#define DEVICE_NODE "/dev/vchar_dev"
#define BUFFER_LENGTH 4              ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM
 
int main(){
	int ret, fd;

	fd = open("/dev/vchar_dev", O_RDWR);             // Open the device with read/write access
	if (fd < 0){
		perror("Failed to open the device...");
		return errno;
	}

	printf("\n\nReading from the device...\n");
	ret = read(fd, receive, BUFFER_LENGTH);        // Read the response from the LKM
	if(ret < 0){
		printf("Failed\n");
		return errno;
	}
	else{
		printf("Random number is %s\n\n",receive);
	}
	close(fd);
	return 0;
}

