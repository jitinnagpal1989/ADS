#include <stdio.h>
#include <sys/time.h> 
#include <string.h>

void GetTimeInSec(int *Seconds,int *Micro);
int MemoryLin();
int MemoryBin();
int DiskLin();
int DiskBin();

int main(int argc, char **argv )
{
    int Seconds = 0;
    int Micro = 0;
    int mode = 0;
    int status = 0;
    char *search_mode= argv[1];
    char *keyfile_name= argv[2];
    char *seekfile_name= argv[3];
    
    // memset(search_mode,'0', sizeof(search_mode));
    // memset(keyfile_name,'0',sizeof(keyfile_name));
    // memset(seekfile_name,'0',sizeof(seekfile_name));
    
    // strcpy(search_mode,argv[1]);
    // strcpy(keyfile_name,argv[2]);
    // strcpy(seekfile_name,argv[3]);
    
    if (strcmp(search_mode,"--mem-lin") == 0)
        mode = 1;
    else if (strcmp(search_mode,"--mem-bin") == 0)
        mode = 2;
    else if (strcmp(search_mode,"--disk-lin") == 0)
        mode = 3;
    else if (strcmp(search_mode,"--disk-bin") == 0)
        mode = 4;
        
    switch (mode)
    {
        case 1:
        status = MemoryLin(*keyfile_name, *seekfile_name);
        break;
        
        case 2:
        status = MemoryBin();
        break;
        
        case 3:
        status = DiskLin();
        break;
        
        case 4:
        status = DiskBin();
        break;
        
        default:
        printf("Invalid search mode entered \n");
        break;
    }
    
    GetTimeInSec(&Seconds,&Micro);
    printf("Seconds: %d\n", Seconds ); 
    printf( "Microseconds: %d\n", Micro );
}

void GetTimeInSec(int *Seconds,int *Micro)
{

    struct timeval ut; 
    gettimeofday( &ut, NULL ); 
    *Seconds = ut.tv_sec;
    *Micro = ut.tv_usec;
}

int MemoryLin(char *key, char *seek)
{   
    size_t num;
    int *key;
    key= (int *)malloc()

}

int MemoryBin(int *key, int *seek)
{
    size_t num;
    FILE *KeyFile, *SeekFile;
    int *k;
    k= (int *)malloc(N *sizeof(int));
    int *s;
