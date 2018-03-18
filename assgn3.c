#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<sys/time.h>

int main(int argc, char* argv[]){
	int mode = 0;
    int status = 0;
    char *merge_method = argv[1];
    char *inputfile_name = argv[2];
    char *outputfile_name = argv[3];

    // char *seekfile_name = argv[3];
    
    if (strcmp(merge_method,"--basic") == 0)
        mode = 1;
    else if (strcmp(merge_method,"--multistep") == 0)
        mode = 2;
    else if (strcmp(merge_method,"--replacement") == 0)
        mode = 3;
    
    switch(mode){
    	case 1: basicMerge(char *inputfile_name, char *outputfile_name);
    			break;
    	case 2: multistepMerge(char *inputfile_name, char *outputfile_name);
    			break;
    	case 3: replacementMerge(char *inputfile_name, char *outputfile_name);
    			break;




    }
}