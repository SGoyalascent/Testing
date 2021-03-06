//Program to test the running status of Coin Converter

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "aes.h"

#define MAXLINE 1024
#define ENCRYPTION_CONFIG_BYTES  16
#define NOUNCE_BYTES_CNT         8
#define FRAME_TIME_OUT_SECS		5 

#define REQ_NO_1  				9
#define REQ_NO_2  				10
#define REQ_NO_3  				11
#define REQ_NO_4  				12
#define REQ_NO_5  				13
#define REQ_NO_6				19
#define REQ_NO_7				20
#define REQ_NO_8				21

uint8_t encrypt_key[ENCRYPTION_CONFIG_BYTES];
uint8_t nounce[NOUNCE_BYTES_CNT];
char execpath[256];
int sockfd;
fd_set select_fds;  
struct timeval timeout;

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
	unsigned char buff[ENCRYPTION_CONFIG_BYTES];
	char path[256];
    strcpy(path,execpath);
	strcat(path,"/encryption_key.bin");
	if ((fp_inp = fopen(path, "rb")) == NULL) {
		printf("encryption_key.bin.bin Cannot be opened , exiting \n");
		return 1;
	}
	if(fread(buff, 1, ENCRYPTION_CONFIG_BYTES, fp_inp)<(ENCRYPTION_CONFIG_BYTES)){
		printf("Configuration parameters missing in encryption_key.bin \n");
		return 1;
	}
	memcpy(encrypt_key,buff,ENCRYPTION_CONFIG_BYTES);
	/*
    for(int i=0;i<ENCRYPTION_CONFIG_BYTES;i++){
		printf("%02x  ",encrypt_key[i]);
	}
	printf("\n");
    */
	fclose(fp_inp);
	return 0;
}

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
    unsigned char buffer[MAXLINE], recv_buffer[MAXLINE];	
    unsigned char buffer_version[MAXLINE]={0,0,1,0,0,15,0,0,0,0,0,0,22,22,0,1,1,0,0,0,0,0,0x3E,0x3E};
    unsigned char buffer_echo[MAXLINE]={0,0,1,0,0,4,0,0,0,0,0,0,22,22,0,1,1,0,0,0,0,0,0x3E,0x3E};
	unsigned char buffer_upgrade_coin[MAXLINE] = {0,0,1,0,0,215,0,0,0,0,0,0,22,22,0,1,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
		        0x35, 0x81, 0xd9, 0xd6, 0x30, 0x1b, 0x4b, 0xc0, 0x48, 0x99, 0xad, 0xef, 0x3c, 0x7d, 0x25, 0xea, 0xd5, 0x51, 0xe2, 0x51, 0x5f, 0x38,
										0x3E, 0x3E};
	
    struct sockaddr_in  servaddr;
    int port_no = 18000;

    const char* ip_address[25] = { "94.130.179.247",  
                                    "141.95.55.85",    
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
                                    "152.67.12.240",   
                                    "104.194.248.208",   
                                    "188.120.241.103",   
                                    "185.37.61.73",  
                                    "193.7.195.250",   
                                    "104.128.190.188",   
                                    "73.12.182.221",   
                                    "190.105.235.113",   
                                    "95.179.220.72",   
                                    "13.127.254.246",   
                                    "104.207.148.199",   
                                    "130.255.77.156",   
                                    "209.205.66.24" };

    getexepath();

    for(int k = 0; k < 25; k++) {
        //int k = 2;
        printf("RAIDA-%d >> \n", k);

        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port_no);
        servaddr.sin_addr.s_addr = inet_addr(ip_address[k]);
//--------------------------------------------------------------
        //ECHO REQUEST
//--------------------------------------------------------------------
        memcpy(buffer,buffer_echo,MAXLINE);	
        buffer[2] = k;  //raida_no
        int n, len=0;
        //printf("Request_echo: ");
        for(int i=0;i<MAXLINE;i++){
            len++;
            //printf("%d,", buffer[i]);
            if(buffer[i]==62 && buffer[i-1]==62){
                break;}
        }
        //printf("\n");
        //printf("\nlen_echo = %d\n",len);

//Loads encrypt key from encryption_key.bin
	
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

        sendto(sockfd, (const char *)buffer, len, MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        set_time_out(FRAME_TIME_OUT_SECS);
        if (select(32, &select_fds, NULL, NULL, &timeout) == 0 ){
            printf("Echo_response: TIME_OUT_ERROR \n"); 
            //continue;    
        }
        else {
            n = recvfrom(sockfd, (char *)recv_buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
            //printf("recv_n: %d\n", n);
    
            int resp_body = n - 12;
            unsigned char *resp_ptr = &recv_buffer[12];
            unsigned char *key_resp = &encrypt_key[0];
            unsigned char *iv_resp = &nounce[0];
            crypt_ctr(key_resp,resp_ptr,resp_body,iv_resp);

            printf("echo_response: ");
            for(int i=0;i<n;i++){	
	            printf("%d,", recv_buffer[i]);}
	        printf("\n");

            if(recv_buffer[2] == 250){
                printf("STATUS_ECHO: SUCCESS\n");
            }
            else if(recv_buffer[2] == 251){
                printf("STATUS_ECHO: FAIL\n");
            }
        }

//--------------------------------------------------------------
        //VERSION REQUEST
//--------------------------------------------------------------------

        memcpy(buffer,buffer_version,MAXLINE);	
        buffer[2] = k;  //raida_no
        len=0;
        //printf("Request_Version: ");
        for(int i=0;i<MAXLINE;i++){
            len++;
            //printf("%d,", buffer[i]);
            if(buffer[i]==62 && buffer[i-1]==62){
                break;}
        }
        //printf("\n");
        //printf("len_version = %d\n",len);

//Loads encrypt key from encryption_key.bin

	
        memset(nounce,0,NOUNCE_BYTES_CNT);
        //We take nouce 5 bytes
        for(int i=0;i < 5;i++){
            nounce[i] = buffer[REQ_NO_1+i];
        }
        j=0;
        //We take nouce 3 bytes 
        for(int i=5; i < 8;i++){
            nounce[i] = buffer[REQ_NO_6+j];
            j++;
        }
        for(int i = 8;i < ENCRYPTION_CONFIG_BYTES; i++) {
            nounce[i] = 0;
        }

        send_req = len - 22;
        req_ptr = &buffer[22];
        key = &encrypt_key[0];
        iv = &nounce[0];

        status  = load_encrypt_key();
        crypt_ctr(key,req_ptr,send_req,iv);

        sendto(sockfd, (const char *)buffer, len, MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        set_time_out(FRAME_TIME_OUT_SECS);
        if (select(32, &select_fds, NULL, NULL, &timeout) == 0 ){
            printf("Version_response: TIME_OUT_ERROR \n");  
            //continue;    
        }
        else {
            n = recvfrom(sockfd, (char *)recv_buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
            //printf("recv_n: %d\n", n);
            printf("version_response: ");
            for(int i=0;i<n;i++){	
	            printf("%d,", recv_buffer[i]);}
	        printf("\n");
            printf("Version: ");
            for(int i=12;i<n;i++){	
	            printf("%c", recv_buffer[i]); }
	        printf("\n");

            if(recv_buffer[2] == 250){
                printf("STATUS_VERSION: SUCCESS\n");
            }
            else if(recv_buffer[2] == 251){
                printf("STATUS_VERSION: FAIL\n");
            }
        }

//--------------------------------------------------------------
        //UPGRADE COIN REQUEST
//--------------------------------------------------------------------
        
        memcpy(buffer,buffer_upgrade_coin,MAXLINE);	
        buffer[2] = k;  //raida_no
        len=0;
        //printf("Request_Upgrade_Coin: ");
        for(int i=0;i<MAXLINE;i++){
            len++;
            //printf("%d,", buffer[i]);
            if(buffer[i]==62 && buffer[i-1]==62){
                break;}
        }
        //printf("\n");
        //printf("len_upgrade_coin = %d\n",len);

//----------------------------------------------------------
//Loads encrypt key from encryption_key.bin
//---------------------------------------------------------
	
        memset(nounce,0,NOUNCE_BYTES_CNT);
        //We take nouce 5 bytes
        for(int i=0;i < 5;i++){
            nounce[i] = buffer[REQ_NO_1+i];
        }
        j=0;
        //We take nouce 3 bytes 
        for(int i=5; i < 8;i++){
            nounce[i] = buffer[REQ_NO_6+j];
            j++;
        }
        for(int i = 8;i < ENCRYPTION_CONFIG_BYTES; i++) {
            nounce[i] = 0;
        }

        send_req = len - 22;
        req_ptr = &buffer[22];
        key = &encrypt_key[0];
        iv = &nounce[0];

        status  = load_encrypt_key();
        crypt_ctr(key,req_ptr,send_req,iv);

//-------------------------------------------------------------------

        sendto(sockfd, (const char *)buffer, len, MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));

        set_time_out(FRAME_TIME_OUT_SECS);
        if (select(32, &select_fds, NULL, NULL, &timeout) == 0 ){
            printf("UPGRADE_COIN_response: TIME_OUT_ERROR \n");  
            //continue;    
        }
        else {
            n = recvfrom(sockfd, (char *)recv_buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
            //printf("recv_n: %d\n", n);
    
            int resp_body = n - 12;
            unsigned char *resp_ptr = &recv_buffer[12];
            unsigned char *key_resp = &encrypt_key[0];
            unsigned char *iv_resp = &nounce[0];
            crypt_ctr(key_resp,resp_ptr,resp_body,iv_resp);

            printf("upgrade_coin_response: ");
            for(int i=0;i<n;i++){	
	            printf("%d,", recv_buffer[i]);}
	        printf("\n");

            if(recv_buffer[2] == 250){
                printf("STATUS_UPGRADE_COIN: SUCCESS\n");
            }
            else if(recv_buffer[2] == 251){
                printf("STATUS_UPGRADE_COIN: FAIL\n");
            }
            else if(recv_buffer[2] == 245){
                printf("STATUS_UPGRADE_COIN: TICKET_NOT_FOUND\n");
            }
        }
        printf("\n");

        close(sockfd);
    }

    return 0;
}