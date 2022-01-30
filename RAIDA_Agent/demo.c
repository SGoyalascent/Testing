
#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
//#include "test.h"

int val;
int test;

int main(int argc, char *args[])
{
	
	
	printf("I am demo.c\n");
    //printf("val = %d   test = %d\n", val, test);
    printf("demo: val = %s   test = %s\n", args[1],args[2]);
	
	return 0;
}
