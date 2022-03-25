// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "aes.h"

#define VER 255
#define PORT     30000
#define MAXLINE 1024
#define ENCRYPTION_CONFIG_BYTES  16
#define NOUNCE_BYTES_CNT         8
#define FRAME_TIME_OUT_SECS		2 

#define REQ_NO_1  				9
#define REQ_NO_2  				10
#define REQ_NO_3  				11
#define REQ_NO_4  				12
#define REQ_NO_5  				13
#define REQ_NO_6				19
#define REQ_NO_7				20
#define REQ_NO_8				21

int sockfd;
fd_set select_fds;  
struct timeval timeout;

//-----------------------------------------------------------
//Set time out for UDP frames
//-----------------------------------------------------------
void set_time_out(unsigned char secs){     
	FD_ZERO(&select_fds);            
	FD_SET(sockfd, &select_fds);           	                                  
	timeout.tv_sec = secs; 
	timeout.tv_usec = 0;
}

int main() {
    int raida_no, coins;
    unsigned char buffer[MAXLINE], recv_buffer[MAXLINE];	
    //sr no >an count
    unsigned char buffer_pown1[MAXLINE]={0,0,1,0,0,0,VER,0,0,0,0,0,22,22,0,1,0,0,0,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										  0,0,10,142, 16, 109, 224, 249, 206, 73, 23, 179, 48, 141, 247, 21, 112, 249, 44,158,16,109,224,249,206,73,23,179,48,157,247,21,112,249,44,
										  0x3E,0x3E};
   // 	same AN and PAN
    unsigned char buffer_pown2[MAXLINE]={0,0,1,0,0,0,VER,0,0,0,0,0,22,22,0,1,0,0,0,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										  0,0,2,142, 16, 109, 224, 249, 206, 73, 23, 179, 48, 141, 247, 21, 112, 249, 44,142, 16, 109, 224, 249, 206, 73, 23, 179, 48, 141, 247, 21, 112, 249, 44,
										  0x3E,0x3E};

   //All failed
    unsigned char buffer_pown3[MAXLINE]={0,0,0,0,0,0,VER,0,0,0,0,0,22,22,0,1,0,0,0,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										  0,0,2,142, 16, 109, 224, 249, 206, 73, 23, 179, 48, 141, 247, 21, 112, 249, 44,158,16,109,224,249,206,73,23,179,48,157,247,21,112,249,44,
										  0,0,3,9,34, 164, 171, 129, 145, 134,  58, 216, 176, 167, 135,  46, 107,   9,  51,158,16,109,224,249,206,73,23,179,48,157,247,21,112,249,44,	
										  0x3E,0x3E};

   //(MIX)
    unsigned char buffer_pown4[MAXLINE]={0,0,1,0,0,0,VER,0,0,0,0,0,22,22,0,1,0,0,0,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										  0,0,2,142, 16, 109, 224, 249, 206, 73, 23, 179, 48, 141, 247, 21, 112, 249, 44,158,16,109,224,249,206,73,23,179,48,157,247,21,112,249,44,
										  0,0,1,164, 164, 166, 172,  32,  71,  52, 103,  17, 116, 161,  69,  81, 106,  29,  54,158,16,109,224,249,206,73,23,179,48,157,247,21,112,249,44,	
										  0x3E,0x3E};

   //ALL pass
    unsigned char buffer_pown5[MAXLINE]={0,0,1,0,0,0,VER,0,0,0,0,0,22,22,0,1,0,0,0,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										  0,0,4,158,16,109,224,249,206,73,23,179,48,157,247,21,112,249,44,158,16,109,224,249,206,73,23,179,48,157,247,21,112,249,45,
										  0,0,3,158,16,109,224,249,206,73,23,179,48,157,247,21,112,249,44,158,16,109,224,249,206,73,23,179,48,157,247,21,112,249,45,	
										  0x3E,0x3E};

    const char* raida_ip[25] = { "87.120.8.249",
                                "23.106.122.6",
                                "172.105.176.86",
                                "85.195.82.169",
                                "198.244.135.236",
                                "88.119.174.101",
                                "209.141.52.193",
                                "179.43.175.35",
                                "104.161.32.116",
                                "66.172.11.25",
                                "194.29.186.69", 
                                "168.235.69.182", 
                                "5.230.67.199",
                                "167.88.15.117",
                                "23.29.115.137",
                                "66.29.143.85",
                                "185.99.133.110",
                                "104.168.162.230", 
                                "170.75.170.4", 
                                "185.215.227.31", 
                                "51.222.229.205", 
                                "31.192.107.132", 
                                "180.235.135.143", 
                                "80.233.134.148", 
                                "147.182.249.132"};


	struct sockaddr_in     servaddr;

	//  memcpy(buffer,buffer_pown1,MAXLINE);	
	//  memcpy(buffer,buffer_pown2,MAXLINE);	
	//  memcpy(buffer,buffer_pown3,MAXLINE);	
	//  memcpy(buffer,buffer_pown4,MAXLINE);	
	  memcpy(buffer,buffer_pown5,MAXLINE);	
	
    for(int k=0; k < 25; k++) {

     // Creating socket file descriptor
	  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
	        perror("socket creation failed");
	        exit(EXIT_FAILURE);
	    }
	    memset(&servaddr, 0, sizeof(servaddr));
	    // Filling server information
	    servaddr.sin_family = AF_INET;
	    servaddr.sin_port = htons(PORT);
	    servaddr.sin_addr.s_addr = INADDR_ANY;
	    servaddr.sin_addr.s_addr = inet_addr(raida_ip[k]);
	    
        //printf("Enter Raida_no.: ");
        //scanf("%d", &raida_no);
        //buffer[2] = raida_no;
        buffer[2] = k;
	    int n, len=0;
	    
        for(int i=0;i<MAXLINE;i++){
            len++;
            //printf("%d ", buffer[i]);
            if(buffer[i]==0x3E && buffer[i-1]==0x3E)
                break;	
	    }
	   //printf("Len = %d\n", len);	

	    sendto(sockfd, (const char *)buffer, len,
	        MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
	            sizeof(servaddr));

        set_time_out(FRAME_TIME_OUT_SECS);
        int tm = select(32, &select_fds, NULL, NULL, &timeout);
        if ( tm == 0 ){
            printf("pown_response_%d: TIME_OUT_ERROR \n", k);
        }
	    //if(tm != 0) {
            else {
            n = recvfrom(sockfd, (char *)recv_buffer, MAXLINE, 
                        MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
            printf("pown_response_%d: ", k);
            for(int i=0;i<n;i++){	
                printf("%d ", recv_buffer[i]);
            }
            printf("\n");
            //break;
        }
	   close(sockfd);
    }
       
  	  return 0;
}