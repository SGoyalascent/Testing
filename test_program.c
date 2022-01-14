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

int main() {
    int sockfd;
    unsigned char buffer[MAXLINE], recv_buffer[MAXLINE];	
    unsigned char buffer_echo[MAXLINE]={0,0,2,0,0,4,2,0,0,0,0,0,22,22,0,1,0,0,0,0,0,0,0x3E,0x3E};
	struct sockaddr_in  servaddr;
    int port_no = 18000;

    const char* ip_address[25] = { "94.130.179.247",  
                                    "185.83.253.56",    
                                    "139.99.155.124",   
                                    "116.203.157.233",   
                                    "95.183.51.104",   
                                    "52.231.186.123",   
                                    "45.12.222.138",   
                                    "54.36.163.57",  
                                    "13.234.55.11",  
                                    "13.233.223.167",   
                                    "134.122.54.207",   
                                    "73.66.181.49",  
                                    "152.67.17.219",   
                                    "104.194.248.208",   
                                    "188.120.241.103",   
                                    "185.37.61.73",  
                                    "193.7.195.250",   
                                    "104.128.190.188",   
                                    "73.12.182.221",   
                                    "190.105.220.72",   
                                    "95.179.220.72",   
                                    "13.127.254.246",   
                                    "104.207.148.199",   
                                    "130.255.77.156",   
                                    "209.205.66.24" };

    
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    for(int k = 0; k < 25; k++) {
        
        memcpy(buffer,buffer_echo,MAXLINE);	
        buffer[2] = k;
        
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port_no);
        servaddr.sin_addr.s_addr = inet_addr(ip_address[k]);

        int n, len=0;
        for(int i=0;i<MAXLINE;i++){
            len++;
            if(buffer[i]==62 && buffer[i-1]==62){
                break;	}
        }
        printf("len_%d = %d\n",k, len);

        sendto(sockfd, (const char *)buffer, len,
            MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
                sizeof(servaddr));
        n = recvfrom(sockfd, (char *)recv_buffer, MAXLINE, 
                    MSG_WAITALL, (struct sockaddr *) &servaddr,
                    &len);
        printf("recv_n_%d: %d\n",k, n);

        for(int i=0;i<n;i++){	
            printf("%d,", recv_buffer[i]);
        }
        printf("\n");
    }
    close(sockfd);

    return 0;
}