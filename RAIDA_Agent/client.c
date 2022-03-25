//Program to test the running status of Coin Converter

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAXLINE 1024
#define FRAME_TIME_OUT_SECS		 1

int sockfd;
fd_set select_fds;  
struct timeval timeout;

int main() {
    unsigned char buffer[MAXLINE], recv_buffer[MAXLINE];	
    unsigned char buffer_echo[MAXLINE]={0,0,2,0,0,4,0,0,0,0,0,0,22,22,0,1,1,0,0,0,0,0,0x3E,0x3E};
	
    struct sockaddr_in  servaddr;
    int port_no = 18000;

    for(int k = 0; k < 5; k++) {

        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port_no);
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

        memcpy(buffer,buffer_echo,MAXLINE);	
        int n, len=0;
        //printf("Request_echo: ");
        for(int i=0;i<MAXLINE;i++){
            len++;
            //printf("%d,", buffer[i]);
            if(buffer[i]==62 && buffer[i-1]==62){
                break;}
        }
        //printf("\n");
        printf("len_echo = %d\n",len);

        sendto(sockfd, (const char *)buffer, len, MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        set_time_out(FRAME_TIME_OUT_SECS);
        if (select(32, &select_fds, NULL, NULL, &timeout) == 0 ){
            printf("Echo_response: TIME_OUT_ERROR \n"); 
            //continue;    
        }
        else {
            n = recvfrom(sockfd, (char *)recv_buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
            printf("recv_n: %d\n", n);
            printf("echo_response: ");
            for(int i=0;i<n;i++){	
	            printf("%d,", recv_buffer[i]);}
	        printf("\n");

            if(recv_buffer[2] == 250){
                printf("STATUS_ECHO: SUCCESS\n");
            }
        }
        close(sockfd);
    }

    return 0;
}