
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define max_no_of_pairs 10000
void sorting();
void display();
void add_record(int key, char* lastname, char* firstname, char* major, char* key_string);
void del_record(int key);
void find_record(int key);
int binarysearch(int target, int left, int right);
int first_fit(int size);
int compare_best(const void *a, const void *b);
int compare_worst(const void *a, const void *b);
int compare_index(const void *a, const void *b);

int i;
int avail_index;
int size;



//predefined structure of index and hole
typedef struct index_S {
    int key; /* Record's key */
    long off; /* Record's offset in file */
}index_S;

typedef struct avail_S{
    int size; /* Hole's size */
    long off; /* Hole's offset in file */
} avail_S;

//these will hold index.bin and avail.bin file contents
index_S index_list[10000];
avail_S avail_list[10000];


FILE *output_avail_file;
FILE *output_index_file;
FILE *output_student_file;
int index_count=0;
int avail_count=0;
const char *order;

int main(int argc, const char *argv[]){
//    if(argc!=3){
//        printf("Invalid number of arguments");
//        exit(-1);
//    }
    
    order = "--best-fit";//argv[1];
    
    
    if( strcmp(order,"--first-fit") != 0 && strcmp(order,"--best-fit") != 0 && strcmp(order,"--worst-fit") != 0 )
    {
        printf("Invalid argument\n");
        exit(-1);
    }
    
    const char *student_file_name = "/Users/unnatiagrawal/Documents/ads/student.db";//argv[2];
    
    if ( ( output_student_file = fopen( student_file_name, "r+b" ) ) == NULL ) {
        output_student_file = fopen( "/Users/unnatiagrawal/Documents/ads/student.db", "w+b" );
        output_avail_file = fopen("/Users/unnatiagrawal/Documents/ads/avail.bin","w+b");
        output_index_file = fopen("/Users/unnatiagrawal/Documents/ads/index.bin","w+b");
    }
    
    if((output_index_file = fopen("/Users/unnatiagrawal/Documents/ads/index.bin","r+b")) == NULL){
        printf("ERROR: Index file doesn't exist\n");
        exit(-1);
    }
    if((output_avail_file = fopen("/Users/unnatiagrawal/Documents/ads/avail.bin","r+b")) == NULL){
        printf("ERROR: Avail file doesn't exist\n");
        exit(-1);
    }
    
    
    fseek(output_index_file,0,SEEK_SET);
    index_S index_obj;
    
    while(fread(&index_obj, sizeof(index_S),1,output_index_file)==1){
        index_list[index_count++] = index_obj;
    }
    fclose(output_index_file);
    
    avail_S hole;
    while(fread(&hole, sizeof(avail_S),1,output_avail_file)==1){
        avail_list[avail_count++] = hole;
    }
    
    fclose(output_avail_file);
    char user_input[500];
    char user_input_copy[500];
    
    
    while(1){
        
        fgets(user_input,100,stdin);
        strcpy(user_input_copy,user_input);
        char* str_token = strtok(user_input_copy," | \n");
        
        if(str_token!=NULL){
            if(strcmp(str_token,"end")==0){
                /*writing index and availability file*/
                output_avail_file = fopen("avail.bin","wb");
                fseek(output_avail_file,0,SEEK_SET);
                for(i=0;i<=avail_count;i++)
                {
                    fwrite(&avail_list[i],sizeof(struct avail_S),1,output_avail_file);
                }
                fclose(output_avail_file);
                
                
                output_index_file = fopen("index.bin","wb");
                fseek(output_index_file,0,SEEK_SET);
                for( i=0;i<=index_count;i++){
                    fwrite(&index_list[i],sizeof(struct index_S),1,output_index_file);
                }
                // fwrite(index_list,sizeof(index_S),index_count,output_index_file);
                fclose(output_index_file);
                
                fclose(output_student_file);
//                if(strcmp(order, "--first-fit")==0){
//                    avail_index = first_fit(size);
                 if(strcmp(order,"--best-fit")==0){
                    qsort(avail_list,avail_count, sizeof(struct avail_S), compare_best);
//                    avail_index = first_fit(size);
                }else if(strcmp(order, "--worst-fit")==0){
                    qsort(avail_list,avail_count, sizeof(struct avail_S), compare_worst);
//                    avail_index = first_fit(size);
                }
                display(); //display index and availability files contents
                exit(0);
                
            }else if(strcmp(str_token,"del")==0){
                str_token = strtok(NULL," \n");
                int key=0;
                if(str_token!=NULL){
                    key = atoi(str_token);
                }
                if(key!=0)
                    del_record(key);
            }else if(strcmp(str_token,"find")==0){
                str_token = strtok(NULL," \n");
                printf("%s",str_token);
                int key=0;
                if(str_token!=NULL){
                    key = atoi(str_token);
                }
                printf("%d",key);
                if(key!=0)
                    find_record(key);
            }else if(strcmp(str_token,"add")==0){
                size = strlen(str_token) + sizeof(int)-1;
                str_token = strtok(NULL," \n");
                int key=0;
                if(str_token!=NULL){
                    key = atoi(str_token);
                }
                if(key==0){
                    //do nothing
                }else{
                    i=0;
                    char* params[4];
                    while(i<4){
                        str_token = strtok(NULL," |");
                        if(str_token==NULL){
                            printf("invalid record entered\n");
                            exit(-1);
                        }
                        params[i++] = str_token;
                    }
                    if(i!=4){
                        printf("invalid record entered\n");
                        exit(-1);
                    }
                    params[3][strlen(params[3]) - 1] = '\0';
                    add_record(key,params[0],params[1],params[2],params[3]);
                }
            }else{
//                printf("Invalid command entered, exiting...\n");
                exit(-1);
            }
            
        }
    }
}

void display(){
    //displaying index contents
    sorting();
    printf("Index:\n");
    for(i=0;i<index_count;i++)
        printf("key=%d: offset=%ld\n",index_list[i].key, index_list[i].off);
    
    int total_holes_space=0;
    
    if(strcmp(order,"--best-fit")==0){
        qsort(avail_list,avail_count, sizeof(struct avail_S), compare_best);
        //                    avail_index = first_fit(size);
    }else if(strcmp(order, "--worst-fit")==0){
        qsort(avail_list,avail_count, sizeof(struct avail_S), compare_worst);
        //                    avail_index = first_fit(size);
    }
    printf("Availability:\n");
    for(i=0;i<avail_count;i++){
        total_holes_space += avail_list[i].size;
        printf("size=%d: offset=%ld\n",avail_list[i].size, avail_list[i].off );
    }
//    printf("Holes space=%d\n",total_holes_space);
    printf("Number of holes: %d\n",avail_count);
    printf("Holes space: %d\n",total_holes_space);

}


void add_record(int key, char* key_string, char* lastname, char* firstname, char* major){
    // printf("%s|",firstname );
    // printf("%s|",lastname );
    // printf("%s|",major );
    // printf("%s",key_string );
    
    // printf("add_record function\n");
    //check if key already exists
    for(i=0;i<index_count;i++){
        if(index_list[i].key==key){
            printf("A record with same key already exists, exiting..\n");
            printf("Record with SID=%d exists\n", key);
            // exit(-1);
            return;
        }
    }
    
    //preparing record string
    char* record = (char*)malloc(strlen(lastname)+strlen(firstname) + strlen(major) + strlen(key_string) +20);
    
    int size = sprintf (record, "%s|%s|%s|%s", key_string, lastname, firstname, major);
    
    // strcpy(record,key);
    // strcpy(record, key_string);
    // strcat(record,"|");
    // strcat(record,lastname);
    // strcat(record,"|");
    // strcat(record,firstname);
    // strcat(record,"|");
    // strcat(record,major);
    // int size = strlen(record);
//    printf("\n%s\n", record);
    
    avail_index=-1;
    long offset;
    
    if(avail_count!=-1){
        if(strcmp(order, "--first-fit")==0){
            avail_index = first_fit(size);
        }else if(strcmp(order,"--best-fit")==0){
            qsort(avail_list,avail_count, sizeof(struct avail_S), compare_best);
            avail_index = first_fit(size);
        }else if(strcmp(order, "--worst-fit")==0){
            qsort(avail_list,avail_count, sizeof(struct avail_S), compare_worst);
            avail_index = first_fit(size);
        }
        if(avail_index==-1){
            // offset = index_list[index_count-1].off+size;
            fseek(output_student_file,0,SEEK_END);
            avail_index = ftell(output_student_file);
        }
        
        index_list[index_count].key=key;
        index_list[index_count].off = avail_index;
        index_count++;
        //
        offset = avail_list[avail_index].off;
        for(i=avail_index;i<avail_count;i++){
            avail_list[i].size = avail_list[i+1].size;
            avail_list[i].off = avail_list[i+1].off;
        }
        avail_count--;
        
    }else{
        fseek(output_student_file,0,SEEK_END);
        offset = ftell(output_student_file);
        index_list[index_count].key=key;
        index_list[index_count].off = offset;
        index_count++;
    }
    //int buffer = {size};
    size = size + sizeof(int);
    //printf("%d\n", size);
    fwrite(&size,sizeof(int),1,output_student_file);
    fwrite(record,sizeof(char),strlen(record),output_student_file);
//    printf("%s",record);
}

void del_record(int key){
    int j;
    // printf("delete record function\n");
    long offset = -1;
    for(i=0;i<=index_count;i++){
        if(index_list[i].key==key){
            offset = index_list[i].off;

            for(j=i;j<index_count;j++){
                index_list[j].key = index_list[j+1].key;
                index_list[j].off = index_list[j+1].off;
            }
            index_count--;
            break;
        }
    }
    if(offset==-1){
        printf("No record with SID=%d exist\n",key);
//        exit(-1);
    }else{
        fseek(output_student_file, offset, SEEK_SET);
        int size=0;
        fread(&size, sizeof(int),1,output_student_file);
        avail_count++;
        avail_list[avail_count].off=offset;
        avail_list[avail_count].size = size;//+sizeof(int);
    }
}


void find_record(int key){
    // printf("find record function\n");
    qsort(index_list,index_count,sizeof(index_S),compare_index);
    int index = binarysearch(key,0,index_count-1);
    if(index==-1){
        printf("No record with SID=%d exists\n",key);
        // exit(-1);
    }else{
        index_S record = index_list[index];
        fseek(output_student_file, record.off, SEEK_SET);
        int size=0;
        fread(&size, sizeof(int),1,output_student_file);
        char* buff = (char*)malloc(size+1);
        fread(buff, 1,size, output_student_file);
//        printf("%s",buff);
        buff[size] = '\0';
        printf("%s\n",buff );
        free(buff);
    }
}

int binarysearch(int target, int left, int right)
{
    int mid;
    if(left>right)
    {
        return -1;
    }
    mid= left+ (right -left)/2;
    
    if(target==index_list[mid].key)
    {
        return mid;
    }
    else
    {
        if(target<index_list[mid].key)
        {
            return binarysearch(target,left,mid-1);
        }
        
        else
        {
            return binarysearch(target,mid+1,right);
        }
    }
}

int first_fit(int size )
{
    int index=-1, new_hole_size;
    for(i=0;i<=avail_count;i++)
    {
        if(avail_list[i].size>=size)
        {
            index=i;
            new_hole_size=avail_list[index].size - size;
            if(new_hole_size>0)
            {
                avail_count++;
                avail_list[avail_count].size= new_hole_size;
                avail_list[avail_count].off=avail_list[index].off+ size;
            }
            break;
        }
        
        
    }
    return index;
}

void sorting(){
    if(strcmp(order, "--first-fit")==0){
                            avail_index = first_fit(size);
    }if(strcmp(order,"--best-fit")==0){
            qsort(avail_list,avail_count, sizeof(struct avail_S), compare_best);
                                avail_index = first_fit(size);
        }else if(strcmp(order, "--worst-fit")==0){
            qsort(avail_list,avail_count, sizeof(struct avail_S), compare_worst);
                                avail_index = first_fit(size);
        }
}
    


int compare_best(const void *a, const void *b)
{
    avail_S *avail_a=(avail_S *) a;
    avail_S *avail_b=(avail_S *) b;
    
    return avail_a->size-avail_b->size;
    
}

int compare_index(const void *a, const void *b)
{
    index_S *avail_a=(index_S *) a;
    index_S *avail_b=(index_S *) b;
    
    return avail_a->key-avail_b->key;
    
}


int compare_worst(const void *a, const void *b)
{
    avail_S *avail_a=(avail_S *) a;
    avail_S *avail_b=(avail_S *) b;

    return avail_b->size-avail_a->size;


    
}
//end
