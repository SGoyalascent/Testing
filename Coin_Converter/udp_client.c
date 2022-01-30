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

 #define VER 255
#define PORT     18000
#define MAXLINE 1024
#define ENCRYPTION_CONFIG_BYTES  16
#define NOUNCE_BYTES_CNT         8
#define FRAME_TIME_OUT_SECS		1

#define REQ_NO_1  				9
#define REQ_NO_2  				10
#define REQ_NO_3  				11
#define REQ_NO_4  				12
#define REQ_NO_5  				13

#define REQ_NO_6				19
#define REQ_NO_7				20
#define REQ_NO_8				21
  //2021-07-15 17:15:15
// Driver code

char execpath[256];
uint8_t encrypt_key[ENCRYPTION_CONFIG_BYTES];
uint8_t nounce[NOUNCE_BYTES_CNT];
struct timeval timeout;
fd_set select_fds; 
int sockfd;

void set_time_out(unsigned char secs){     
	FD_ZERO(&select_fds);            
	FD_SET(sockfd, &select_fds);           	                                  
	timeout.tv_sec = secs; 
	timeout.tv_usec = 0;
}

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
	
int load_encrypt_key(){

	FILE *fp_inp = NULL;
	int i = 0;
	unsigned char buff[ENCRYPTION_CONFIG_BYTES];
	char path[256];
	strcpy(path,execpath);
	strcat(path,"/Data/encryption_key.bin");
	printf("------------------------------\n");
	printf("ENCRYPTION CONFIG KEY ..\n");
	printf("------------------------------\n");
	if ((fp_inp = fopen(path, "rb")) == NULL) {
		printf("encryption_key.bin.bin Cannot be opened , exiting \n");
		return 1;
	}
	if(fread(buff, 1, ENCRYPTION_CONFIG_BYTES, fp_inp)<(ENCRYPTION_CONFIG_BYTES)){
		printf("Configuration parameters missing in encryption_key.bin \n");
		return 1;
	}
	memcpy(encrypt_key,buff,ENCRYPTION_CONFIG_BYTES);
	for(i=0;i<ENCRYPTION_CONFIG_BYTES;i++){
		printf("%02x  ",encrypt_key[i]);
	}
	printf("\n");
	fclose(fp_inp);

	return 0;
}

int main() {
    int i=0;
    unsigned char buffer[MAXLINE], recv_buffer[MAXLINE];	
    unsigned char buffer_version[MAXLINE]={0,0,1,0,0,15,2,0,0,0,0,0,22,22,0,1,1,0,0,0,0,0,0x3E,0x3E};
    unsigned char buffer_echo[MAXLINE]={0,0,1,0,0,4,2,0,0,0,0,0,22,22,0,1,1,0,0,0,0,0,0x3E,0x3E};
	unsigned char buffer_upgrade_coin[MAXLINE] = {0,0,1,0,0,215,0,0,0,0,0,0,22,22,0,1,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										0x35, 0x81, 0xd9, 0xd6, 0x30, 0x1b, 0x4b, 0xc0, 0x48, 0x99, 0xad, 0xef, 0x3c, 0x7d, 0x25, 0xea, 0xd5, 0x51, 0xe2, 0x51, 0x5f, 0x38,
										0x3E, 0x3E};
/*
 unsigned char buffer_upgrade_coin[MAXLINE] = {0,0,2,0,0,215,2,0,0,0,0,0,22,22,0,1,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0x3E,0x3E};
*/

// 3c 70 a0 70 49 01 3e 77 be df 14 f4 e5 21 5d ad 62 d1 a1 61 46 e4
// 0xe4,0x46,0x61,0xa1,0xd1,0x62,0xad,0x5d,0x21,0xe5,0xf4,0x14,0xdf,0xbe,0x77,0x3e,0x01,0x49,0x70,0xa0,0x70,0x3c


		struct sockaddr_in     servaddr;
			
	//   memcpy(buffer,buffer_version,MAXLINE);			
	//	memcpy(buffer,buffer_echo,MAXLINE);	
		memcpy(buffer, buffer_upgrade_coin, MAXLINE);

	    // Creating socket file descriptor
	    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
	        perror("socket creation failed");
	        exit(EXIT_FAILURE);
	    }
		else {
			printf("Socket Creation Successful\n");
		}
	  
	    memset(&servaddr, 0, sizeof(servaddr));
	    // Filling server information
	    servaddr.sin_family = AF_INET;
	    servaddr.sin_port = htons(PORT);
//	    servaddr.sin_addr.s_addr = INADDR_ANY;
	   servaddr.sin_addr.s_addr = inet_addr("141.95.55.85");

	      
	    int n, len=0;
	     for(i=0;i<MAXLINE;i++){
		len++;
		//printf("%d,",buffer[i]);
		if(buffer[i]==62 && buffer[i-1]==62)
			break;	
	    }
	   printf("Len = %d %d\n", len,i);
//----------------------------------------------------------
//Loads encrypt key from encryption_key.bin
//--------------------------------------------------------- 
	
	getexepath();

	memset(nounce,0,NOUNCE_BYTES_CNT);
	//We take nouce 5 bytes
	for(int i=0;i < 5;i++){
		nounce[i] = buffer[REQ_NO_1+i];
	}
	int j=0;
	//We take nouce 3 bytes 
	for(int i=5; i < 8;i++){
		nounce[i] = buffer[REQ_NO_6+j];
		j++;
	}
	for(int i = 8;i < ENCRYPTION_CONFIG_BYTES; i++) {
		nounce[i] = 0;
	}

	int send_req = len - 22;
	unsigned char *req_ptr = &buffer[22];
	unsigned char *key = &encrypt_key[0];
	unsigned char *iv = &nounce[0];

	int status  = load_encrypt_key();
	crypt_ctr(key,req_ptr,send_req,iv);

//-----------------------------------------------------------------------
	printf("buffer: ");
	for(int i=0; i < len; i++) {
		printf("%d  ", buffer[i]);
	}
	printf("\n");

	    sendto(sockfd, (const char *)buffer, len,
	        MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
	            sizeof(servaddr));
/*
		set_time_out(FRAME_TIME_OUT_SECS);
        if (select(32, &select_fds, NULL, NULL, &timeout) == 0 ){
            printf("Time out error \n"); 
            //printf("STATUS: FAIL\n");
            exit(EXIT_FAILURE);    
        }
		else {
            n = recvfrom(sockfd, (char *)recv_buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
            //printf("STATUS: SUCCESS\n");
        } */

		
	    n = recvfrom(sockfd, (char *)recv_buffer, MAXLINE, 
	                MSG_WAITALL, (struct sockaddr *) &servaddr,
	                &len); 

		printf("n: %d\n", n);
	   for(i=0;i<n;i++){	
	     printf("%d,", recv_buffer[i]);
	   }
	   printf("\n");

	int resp_body = n - 12;
	unsigned char *resp_ptr = &recv_buffer[12];
	unsigned char *key_resp = &encrypt_key[0];
	unsigned char *iv_resp = &nounce[0];

	crypt_ctr(key_resp,resp_ptr,resp_body,iv_resp);

	close(sockfd);
  	return 0;
}