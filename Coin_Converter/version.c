// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "aes.h"


char execpath[256];

void getexepath()
{
	char buff[256];
	int count = readlink( "/proc/self/exe", buff, 256);
	int i=0,slash_pos;
	while(buff[i]!='\0'){
		if(buff[i]=='/'){
			slash_pos = i;
		}
		i++;
	}	
	strncpy(execpath,buff,slash_pos);
}

void execute_version(){
	FILE *fp_inp = NULL;
	char path[256];
	unsigned char response[256];
	unsigned int index = 0;
	char c;
	strcpy(path,execpath);
	strcat(path,"/version.txt");
	printf("VERSION Command \n");
	if ((fp_inp = fopen((const char *)path, "r")) == NULL) {
		printf("version.txt cannot be opened , exiting \n");
		send_err_resp_header(FAIL);
		return ;
	}
	while ((c = fgetc(fp_inp)) != EOF){
		response[index] = c;
		printf("%d ", )
		index++;
    }
	fclose(fp_inp);
}

int main() {

	getexepath();






}