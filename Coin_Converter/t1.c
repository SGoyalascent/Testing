#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

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


int main() {
    
    getexepath();
    unsigned char raida_1, raida_2;
    int raida;


    FILE *fp_inp=NULL;
    int size=0,ch;
    unsigned char buff[24];
    char path[256];
    strcpy(path,execpath);
    strcat(path,"/raida_no.txt");
    if ((fp_inp = fopen(path, "r")) == NULL) {
        printf("raida_1 Cannot be opened , exiting \n");
    }
    while( ( ch = fgetc(fp_inp) ) != EOF ){
        size++;
    }
    fclose(fp_inp);
    fp_inp = fopen(path, "r");
    if(fread(buff, 1, size, fp_inp)<size){
        printf("Configuration parameters missing in raida_no.txt \n");
    }
    if(size >= 2){
        raida_1 = (buff[0]-48)*10;   
        raida_1+= (buff[1]-48);  
    }else{
        raida_1=buff[0]-48;
    }

    printf("Raida_1 :- %d \n", raida_1);  //238
    fclose(fp_inp);

    FILE *myfile = fopen(path, "r");
    if(myfile == NULL) {
         printf("raida2 Cannot be opened , exiting \n");
    }
    fscanf(myfile, "%255s", &raida_2);
    fclose(myfile);

    printf("Raida_2 :- %c \n", raida_2);  //2
    printf("Raida_3 :- %d \n", raida_2-48);  // 50

    FILE *myfil = fopen(path, "r");
    if(myfil == NULL) {
         printf("raida_no4 Cannot be opened , exiting \n");
    }
    fscanf(myfil, "%d", &raida);
    fclose(myfil);

    printf("Raida_4 :- %d \n", raida);   //2

}


