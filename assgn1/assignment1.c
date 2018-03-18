#include <stdio.h>
#include <sys/time.h> 
#include <string.h>
#include <stdlib.h>

/* declaring functions */
int OnDiskSearch(char *key, char *seek, int mode);
int InMemorySearch(char *key, char *seek, int mode);
int binarySearch(int *array, int last, int first, int toFind);
int binarySearchOnDisk(FILE *KEY, int last, int first, int toFind);
void PrintAndCleanup(long TimeDifference1,long TimeDifference2, FILE* KeyFile, FILE* SeekFile);

int main(int argc, char **argv )
{
    int mode = 0;
    int status = 0;
    char *search_mode = argv[1];
    char *keyfile_name = argv[2];

    char *seekfile_name = argv[3];
    
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
        case 2:
        status = InMemorySearch(keyfile_name, seekfile_name,mode);
        if ( status != 0 )
        {
            printf( "Failed in InMemorySearch \n");
            return 1;
        }
        break;
        
        case 3:
        case 4:
        status = OnDiskSearch(keyfile_name, seekfile_name,mode);
        if ( status != 0 )
        {
            printf( "Failed in OnDiskSearch \n");
            return 1;
        }
        break;
        
        default:
        printf("Invalid search mode entered \n");
        return 1;
        break;
    }
    
    return 0;
}


int InMemorySearch(char *key, char *seek, int mode)
{
   FILE *KeyFile, *SeekFile;
    int *key_array = NULL;
    int *seek_array = NULL;
    int *hit = NULL;
    int value = 0;
    int Counter = 0;
    int seeksize = 0;
    int keysize = 0;
    int i=0;
    long TimeDifference1 = 0;
    long TimeDifference2 = 0;

    
    struct timeval KeyOpenTime;
    struct timeval KeyFinishTime;
    struct timeval BeforeBinarySearch;
    struct timeval AfterBinarySearch;
    struct timeval BeforeLinearSearch;
	struct timeval AfterLinearSearch;
    
    SeekFile = fopen(seek,"rb");
    if ( !SeekFile )
    {
        printf("Failed to open SeekFile \n");
        return 1;
    }
    
    i=0;
    while (1)
    {
		fseek(SeekFile, i*sizeof(int), SEEK_SET);
		fread(&value, sizeof(int), 1, SeekFile);
		if(feof(SeekFile) != 0)
			break;
		Counter++;
		i++;
	}
    
    seek_array=(int *) malloc(Counter * sizeof(int));
    i = 0;
	while (1)
	{
		fseek(SeekFile, i*sizeof(int), SEEK_SET);
	    fread(&value, sizeof(int), 1, SeekFile);
		if(feof(SeekFile) != 0)
			break;
		seek_array[i] = value;
		i++;
	}
	
	seeksize = Counter;
	Counter = 0;
	

	gettimeofday(&KeyOpenTime, NULL);
	
	KeyFile = fopen(key, "rb");
	if ( !KeyFile )
    {
        printf("Failed to open KeyFile \n");
        return 1;
    }
    
    i = 0;
    while (1)
    {
		fseek(KeyFile, i*sizeof(int), SEEK_SET);
		fread(&value, sizeof(int), 1, KeyFile);
		if(feof(KeyFile) != 0)
			break;
		Counter++;
		i++;
	}
	
	key_array = (int *) malloc(Counter * sizeof(int));
	i = 0;
	while (1)
	{
		fseek(KeyFile, i*sizeof(int), SEEK_SET);
		fread(&value, sizeof(int), 1, KeyFile);
		if(feof(KeyFile) != 0)
			break;
		key_array[i] = value;
		i++;
	}
	keysize = Counter;
	
	gettimeofday(&KeyFinishTime, NULL);
	
	TimeDifference1 = (KeyFinishTime.tv_sec - KeyOpenTime.tv_sec);
	TimeDifference2 = (KeyFinishTime.tv_usec - KeyOpenTime.tv_usec);
	
	hit = (int *) malloc (seeksize * sizeof(int));
	
	int position = 0;
	int toFind = 0;
	
	if(mode == 2)
	{
	    /* for Binary Search */
		gettimeofday(&BeforeBinarySearch, NULL);
		for(i=0; i<seeksize; i++)
	    {
    		toFind = seek_array[i];
		    position = binarySearch(key_array, 0, keysize-1, toFind);
		    if(position == -1)
		    {
    			hit[i] = 0;
		    } 
		    else
		    {
			    hit[i] = 1;
		    }
	    }
		gettimeofday(&AfterBinarySearch, NULL);
		
		TimeDifference1 = (AfterBinarySearch.tv_sec - BeforeBinarySearch.tv_sec);
		 TimeDifference2= (AfterBinarySearch.tv_usec - BeforeBinarySearch.tv_usec);
	}
	else
	{
	    /* For Linear search */
		gettimeofday(&BeforeLinearSearch, NULL);
		for(i=0; i<seeksize; i++)
	    {
			toFind = seek_array[i];
			int j=0;
			for(j=0; j<keysize; j++)
			{
				if(key_array[j] == toFind)
				{
					hit[i] = 1;
					break;	
				}
				else
				{
					hit[i] = 0;
				}
			}
		}
		gettimeofday(&AfterLinearSearch, NULL);
		
		TimeDifference1 = (AfterLinearSearch.tv_sec - BeforeLinearSearch.tv_sec);
		TimeDifference2= (AfterLinearSearch.tv_usec - BeforeLinearSearch.tv_usec);
	}
	
	/* for displaying results on screen */

	for (i=0; i<seeksize; i++)
	{
		if(hit[i] == 1)
		{
			printf( "%12d: Yes\n", seek_array[i] );
		} 
		else if(hit[i] == 0)
		{
			printf( "%12d: No\n", seek_array[i] );
		}
	}
	
	PrintAndCleanup(TimeDifference1,TimeDifference2,KeyFile,SeekFile);
	return 0;
}

void PrintAndCleanup(long TimeDifference1,long TimeDifference2, FILE* KeyFile, FILE* SeekFile){
	/* printing time difference in decimals. Dividing again by Micros in a second */
	// printf("Time: %f\n\n", (float)TimeDifference/1000000);
	printf( "Time: %ld.%06ld\n", TimeDifference1, TimeDifference2 );
    fclose(KeyFile);
    fclose(SeekFile);
}

int OnDiskSearch(char *key, char *seek, int mode)
{
    FILE *KeyFile, *SeekFile;
    int *key_array = NULL;
    int *seek_array = NULL;
    int *hit = NULL;
    int value = 0;
    int Counter = 0;
    int seeksize = 0;
    int keysize = 0;
    long TimeDifference1 = 0;
    long TimeDifference2 = 0;

    int i=0;
    
    struct timeval BeforeBinarySearch;
    struct timeval AfterBinarySearch;
    struct timeval BeforeLinearSearch;
	struct timeval AfterLinearSearch;

    SeekFile = fopen(seek,"rb");
    if ( !SeekFile )
    {
        printf("Failed to open SeekFile \n");
        return 1;
    }
    
    for(i=0;;i++)
    {
		fseek(SeekFile, i*sizeof(int), SEEK_SET);
		fread(&value, sizeof(int), 1, SeekFile);
		if(feof(SeekFile) == 0)
		{
			Counter++;
		} 
		else
		    break;
	}
    
    seek_array=(int *) malloc(Counter * sizeof(int));
    //fseek(SeekFile, i*sizeof(int), SEEK_SET);
	for(i=0;;i++)
	{
		fseek(SeekFile, i*sizeof(int), SEEK_SET);
	    fread(&value, sizeof(int), 1, SeekFile);
		if(feof(SeekFile) == 0)
		{
			seek_array[i] = value;
		}
		else
		    break;
	}
	
	seeksize = Counter;
	Counter = 0;
	
	hit = (int *) malloc (seeksize * sizeof(int));
	
	int position = 0;
	int toFind = 0;
	int flag = 0;
	
	if(mode == 4)
	{
	    /* for Binary Search */
		gettimeofday(&BeforeBinarySearch, NULL);
	    
	    KeyFile = fopen(key, "rb");
    	if ( !KeyFile )
        {
            printf("Failed to open KeyFile \n");
            return 1;
        }

        //fseek(KeyFile, i*sizeof(int), SEEK_SET);
        
        for(i=0;;i++)
        {
    		fseek(KeyFile, i*sizeof(int), SEEK_SET);
    		fread(&value, sizeof(int), 1, KeyFile);
    		if(feof(KeyFile) != 0)
    			break;
    		Counter++;
    	}
    	keysize = Counter;
    	for(i=0;i<seeksize;i++)
    	{
			toFind = seek_array[i];
			position = binarySearchOnDisk(KeyFile, 0, keysize-1, toFind);
			if(position == -1)
			{
				hit[i] = 0;
			}
			else
			{
				hit[i] = 1;
			}
		}
		gettimeofday(&AfterBinarySearch, NULL);
		
		TimeDifference1 = (AfterBinarySearch.tv_sec - BeforeBinarySearch.tv_sec);
		TimeDifference2=(AfterBinarySearch.tv_usec - BeforeBinarySearch.tv_usec);
	}
	else
	{
	    /* For Linear search */
    	gettimeofday(&BeforeLinearSearch, NULL);
			
	    KeyFile = fopen(key, "rb");
	    if ( !KeyFile )
	    {
	        printf( "Error in opening KeyFile \n");
	        return -1;
	    }
	    
	    // fseek(KeyFile, sizeof(int), SEEK_SET);
	    for(i=0;i<seeksize;i++)
	    {
			toFind = seek_array[i];
			// fseek(KeyFile, i*sizeof(int), SEEK_SET);
			int j=0;
			for(j=0;;j++)
			{
				fseek(KeyFile, j*sizeof(int), SEEK_SET);
				fread(&value, sizeof(int), 1, KeyFile);
				if(feof(KeyFile) == 0)
				{
					if(toFind == value)
					{
						hit[i] = 1;
						break;
					}
					else
					{
						hit[i] = 0;
					}
				}
				else
				{
					break;
				} 
			}
		}
		gettimeofday(&AfterLinearSearch, NULL);
		
		TimeDifference1 =  (AfterLinearSearch.tv_sec - BeforeLinearSearch.tv_sec);
		TimeDifference2= (AfterLinearSearch.tv_usec - BeforeLinearSearch.tv_usec);
	}
	
	/* for displaying results on screen */
	for (i=0; i<seeksize; i++)
	{
		if(hit[i] == 1)
		{
			printf( "%12d: Yes\n", seek_array[i] );
		} 
		else if(hit[i] == 0)
		{
			printf( "%12d: No\n", seek_array[i] );
		}
	}
	
	PrintAndCleanup(TimeDifference1,TimeDifference2,KeyFile,SeekFile);
    
    return 0;
}

int binarySearch(int *array, int last, int first, int toFind)
{
    int mid = 0;
    
	if(first < last)
		return -1;
	mid = first + (last - first)/2;
	if(array[mid] == toFind)	
	    return mid;
	else if (toFind > array[mid])
	    return binarySearch(array, mid+1, first, toFind);
	else if (toFind < array[mid])
	    return binarySearch(array, last, mid-1, toFind);
}

int binarySearchOnDisk(FILE *KEY, int last, int first, int toFind)
{
    int middle = 0;
    int temp;
	if(first < last)
		return -1;
	
	middle = first + (last - first)/2;
	
	
	fseek(KEY, middle * sizeof(int), SEEK_SET);
	fread(&temp, sizeof(int), 1, KEY);
	
	if(toFind == temp)	
	    return middle;
	else if(toFind > temp)
	    return binarySearchOnDisk(KEY, middle+1, first, toFind);
	else if(toFind < temp)
	    return binarySearchOnDisk(KEY, last, middle-1, toFind);
	
}


	