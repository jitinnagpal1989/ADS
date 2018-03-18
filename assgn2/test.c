#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define max_input_size 256
#define max_pairs 10000

typedef struct index_S {
	int key;
	long offset;
	}index_S;

typedef struct avail_S{ 
	int size; /* Hole's size */ 
	long off; /* Hole's offset in file */ 
	} avail_S;

int compare_index(const void *a, const void *b)
{
	index_S *index_a=(index_S *) a;
	index_S *index_b=(index_S *) b;
	return index_a->key-index_b->key;

}
int compare_avail_best(const void *a, const void *b)
{
	avail_S *avail_a=(avail_S *) a;
	avail_S *avail_b=(avail_S *) b;
	
	return avail_a->size-avail_b->size;

}
int compare_avail_worst(const void *a, const void *b)
{
	avail_S *avail_a=(avail_S *) a;
	avail_S *avail_b=(avail_S *) b;
	
	return avail_b->size-avail_a->size;

}

int first_fit(struct avail_S *avail_list,int rec_size,int is_add,int* avail_list_size)
{	
	int i,size_diff,avail_result,is_avail=0;
	//printf("indisde first fit\n");
	if(is_add==1)
	{
		for(i=0;i<=*avail_list_size;i++)
 		{
			if(avail_list[i].size>=(rec_size))
			{
			avail_result=i;
			is_avail=1;
			size_diff=avail_list[avail_result].size - (rec_size); //size of created hole
			//printf("\nsize diff :%d",size_diff);
			if(size_diff>0)
			{
			++(*avail_list_size);							 //increment end of availibility list
			avail_list[*avail_list_size].size=size_diff;  //append hole to end of list
			avail_list[*avail_list_size].off=avail_list[avail_result].off + rec_size; //add offset to end of list
			//printf("\n%d\n",i);
			
			}
			//return avail_list[i];
			break;
			}
		}
	
		if (is_avail==1)
		{
		return avail_result;
		}
		else 
		{
			return -1;
		}
		//return avail_list[i];
		// return avail_list[avail_list_size+1];

		
	}
	/*else if(is_add==0)
	{
		avail_list_size++;
		avail_list[avail_list_size].size=rec_size;
		avail_list[avail_list_size].off=offset?

	}*/
	exit(1);
}

int best_fit(struct avail_S *avail_list,int rec_size,int is_add,int* avail_list_size)
{	
	int i,size_diff,avail_result,is_avail=0;
		//printf("indisde best fit\n");

for(i=0;i<=*avail_list_size;i++)
 		{
			if(avail_list[i].size>=(rec_size))
			{
			avail_result=i;
			is_avail=1;
			size_diff=avail_list[avail_result].size - (rec_size); //size of created hole
			//printf("\nsize diff :%d",size_diff);
			if(size_diff>0)
			{
			++(*avail_list_size);							 //increment end of availibility list
			avail_list[*avail_list_size].size=size_diff;  //append hole to end of list
			avail_list[*avail_list_size].off=avail_list[avail_result].off + rec_size; //add offset to end of list
			//printf("\n%d\n",i);
			
			}
			//return avail_list[i];
			break;
			}
		}
	
		if (is_avail==1)
		{
		return avail_result;
		}
		else 
		{
			return -1;
		}
		//return avail_list[i];
		// return avail_list[avail_list_size+1];
}

int worst_fit(struct avail_S *avail_list,int rec_size,int is_add,int* avail_list_size)
{	
	int i,size_diff,avail_result,is_avail=0;
		//printf("indisde worst fit\n");

for(i=0;i<=*avail_list_size;i++)
 		{
			if(avail_list[i].size>=(rec_size))
			{
			avail_result=i;
			is_avail=1;
			size_diff=avail_list[avail_result].size - (rec_size); //size of created hole
			//printf("\nsize diff :%d",size_diff);
			if(size_diff>0)
			{
			++(*avail_list_size);							 //increment end of availibility list
			avail_list[*avail_list_size].size=size_diff;  //append hole to end of list
			avail_list[*avail_list_size].off=avail_list[avail_result].off + rec_size; //add offset to end of list
			//printf("\n%d\n",i);
			
			}
			//return avail_list[i];
			break;
			}
		}
	
		if (is_avail==1)
		{
		return avail_result;
		}
		else 
		{
			return -1;
		}
		//return avail_list[i];
		// return avail_list[avail_list_size+1];
}



int add_exist(char *uid,struct index_S *key_index,int *index_list_size)
{
	int i,u_id;
	u_id=atoi(uid);
	//printf("\nUID in add_exist: %d\n",u_id );
	for(i=0;i<=*index_list_size;i++)
	{
		if(key_index[i].key==u_id)			//check if key already exists
		{
		return -1;	
		}
	}
return 1;
}

int del_exist (int u_id,struct index_S *key_index,int *index_list_size,int is_del)
{
	int i,location;
	long offset=-1;

	//u_id=atoi(uid);
	//printf("\nUID in del_exist: %d\n",u_id );
	for(i=0;i<=*index_list_size;i++)
	{
		if(key_index[i].key==u_id)			//check if key  exists
		{
		location=i;
		offset=key_index[i].offset;
		if(is_del==1)
		{
			for(i=location;i<((*index_list_size));i++)
			{  // printf("\ninside location for\n");
				key_index[i].offset=key_index[i+1].offset;
				key_index[i].key=key_index[i+1].key;
			}
	
			(*index_list_size)--;

		}
		break;
		
		}
	
	}
	//printf("\nlocation %d\n",location );
	
		
	/*for(i=0;i<=*index_list_size;i++)
		printf("\nafter del offset:%lu  key= %d\n",key_index[i].offset,key_index[i].key );
*/

	return offset;	
		
	



	/*key_index[*index_list_size].key= u_id;
	
	(*index_list_size)++;*/
}


int main(int argc, const char *argv[])
{
index_S index_S[500];
avail_S avail_S[500];
char input[500],input_copy[500],*buff;
char *inputParse[3],*input_tokens,*temp;
int i,j=0, rec_size,prev_rec_size,rec_key,k=0,is_add=0,avail_result,is_best=0,is_worst=0;
long rec_off,position_offset,del_return;
int avail_list_size=-1,index_list_size=-1,del_key; 
int add_return;
FILE *fp,*out_index,*out_avail;
char buf[1000];
int rec_siz,hole_space=0,avail_holes=0;
int index_end,avail_end;
/*if file exists then 
	open it and read avail_S
else
	*/
//avail_S[0].size=10000;
//avail_S[0].off=0;

/*if there exist avail_S and index_S files, read them and load data into structures
	update avail_list_size and max_input_size*/

/*User menu
Coninutes in a while loop till user exits
*/
if(access(argv[2],F_OK)!=-1)
{
out_index=fopen("index.db","rb");
		if(out_index)
		{
				
				fseek(out_index,0,SEEK_END);
				index_end=ftell(out_index);
				fseek(out_index,0,SEEK_SET);
		    	//for(k=0;k<=index_list_size;k++)
		    	k=0;
		    	while(ftell(out_index)<index_end)
		    	{
		    	fread(&index_S[k],sizeof(struct index_S),1,out_index);
		    	k++;
		    	index_list_size++;
		    	}

		    	fclose(out_index);
				/*	for(k=0;k<=index_list_size;k++)
							{
								printf("\nindex:%d  %lu : %d",k,index_S[k].offset,index_S[k].key);
							}
*/
		}
out_avail=fopen("avail.db","rb");
		if(out_avail)
		{
				
				fseek(out_avail,0,SEEK_END);
				avail_end=ftell(out_avail);
				fseek(out_avail,0,SEEK_SET);
		    	//for(k=0;k<=index_list_size;k++)
		    	k=0;
		    	while(ftell(out_avail)<avail_end)
		    	{
		    	fread(&avail_S[k],sizeof(struct avail_S),1,out_avail);
		    	k++;
		    	avail_list_size++;
		    	}

		    	fclose(out_avail);
					/*for(k=0;k<=avail_list_size;k++)
							{
								printf("\navail:%d  %lu : %d",k,avail_S[k].off,avail_S[k].size);
							}*/

		}
}


do{
	/*printf("\nMENU\n");
	printf("Enter your choice:\n");
	printf("1.Read");
	printf("\n2.Write");
	printf("\n3.End\n");
	//scanf("%s",input);*/

	fgets(input,max_input_size,stdin);
	//printf("\n%s : %lu\n",input,strlen(input));
	
	j=0;
	//printf("\nhere\n");
	
    strcpy(input_copy,input);

	input_tokens=strtok(input_copy," \n");
	
	while(input_tokens != NULL)
	{
		inputParse[j++]=input_tokens;
		//printf("\n%s",input_tokens);
		input_tokens=strtok(NULL," ");

	}
	


if(strcmp(inputParse[0],"end")==0)
		    {
		    
		    	
		    	out_index=fopen("index.db","wb");
		    	fseek(out_index,0,SEEK_SET);
		    	for(k=0;k<=index_list_size;k++)
		    	{
		    		fwrite(&index_S[k],sizeof(struct index_S),1,out_index);
		    	}
		    	fclose(out_index);
		    	

		    	
		    	
		    	out_avail=fopen("avail.db","wb");
		    	fseek(out_avail,0,SEEK_SET);
		    	for(k=0;k<=avail_list_size;k++)
		    	{
		    		fwrite(&avail_S[k],sizeof(struct avail_S),1,out_avail);
		    	}
		    	fclose(out_avail);
		    	

		    	printf("\nIndex:");
		    	for(k=0;k<=index_list_size;k++)
							{
								printf("\nkey=%d: offset=%lu",index_S[k].key,index_S[k].offset);
							}
				printf("\nAvailability:");
				for(k=0;k<=avail_list_size;k++)
							{
								printf("\nsize=%d: offset=%lu",avail_S[k].size,avail_S[k].off);
							avail_holes++;
							hole_space +=avail_S[k].size;
							}
				printf("\nNumber of holes: %d",avail_holes);
				printf("\nHole space: %d",hole_space);




		   // printf("\nquitting prog\n");
			return 0;
		    }


else if(strcmp(inputParse[0],"add")==0)
			{
			is_add=1;
			//printf("\ninside compare\n");
			rec_size=strlen(inputParse[2])+sizeof(int)-1; //keep the size in mind

			if(avail_list_size>=0)
			{
			if (strcmp(argv[1],"--first-fit")==0)
					{
							avail_result=first_fit(avail_S,rec_size,is_add, &avail_list_size);
							//printf("\nfirst fit\n");
						
					}
			else if(strcmp(argv[1],"--best-fit")==0)
					{
							avail_result=best_fit(avail_S,rec_size,is_add, &avail_list_size);
							is_best=1;
					}
			else if(strcmp(argv[1],"--worst-fit")==0)
					{
							avail_result=worst_fit(avail_S,rec_size,is_add, &avail_list_size);
							is_worst=1;
					}
			}

			add_return=add_exist(inputParse[1],index_S,&index_list_size);


			if(add_return==-1)
					{
							printf("\nRecord with SID=%s exists",inputParse[1]);

						
					}
			else
					{		int u_id=atoi(inputParse[1]);
							index_list_size++;
							//printf("\nindex_list_size:%d\navail list size=%d\nrec_size=%d\nprev rec size=%d\n",index_list_size,avail_list_size,rec_size,prev_rec_size );
							index_S[index_list_size].key= u_id;
							//index_list_size++;
							
							if((fp=fopen(argv[2],"r+b"))==NULL)
								{
									fp=fopen(argv[2],"w+b");
									//printf("\n couldnt create file");
									//exit(1);
								}


							if(avail_list_size>=0)	//if there are holes
								{	//printf("inside avail list size-avail_result=%d",avail_result);
									if(avail_result==-1) //no holes large enuf 
									{
										position_offset=index_S[index_list_size-1].offset+rec_size;
									}
									else
									{
										position_offset=avail_S[avail_result].off;
										for(k=avail_result;k<(avail_list_size);k++)
										{
											avail_S[k].size=avail_S[k+1].size;
											avail_S[k].off=avail_S[k+1].off;
										}
										avail_list_size--;
									if(is_best==1)
									{
										qsort(avail_S,(avail_list_size),sizeof(struct avail_S),compare_avail_best);

									}
									if(is_worst==1)
									{
										qsort(avail_S,(avail_list_size),sizeof(struct avail_S),compare_avail_worst);
									}

									}
									/*for(k=0;k<=avail_list_size;k++)
										{
										printf("\navail:  %lu : %d",avail_S[k].off,avail_S[k].size);
										}*/
								}
							else if(index_list_size==0)   //adding first element
							{	//printf("\ninside offset =0\n");
								position_offset=0;
							}
							else
							{	//printf("\ninside else offset >0\n");
								fseek(fp, 0, SEEK_END);
								position_offset = ftell(fp);
							}
							prev_rec_size=rec_size;
							index_S[index_list_size].offset=position_offset;
							
							/*printf("\nbefore qsort:");
							for(k=0;k<=index_list_size;k++)
							{
								printf("\nindex:%d  %lu : %d",k,index_S[k].offset,index_S[k].key);
							}*/

							//printf("\n%d\n",rec_size);

							//char *writeData=malloc(sizeof(inputParse[2]));
							//sprintf(writeData,"%d",rec_size);
							//strcpy(writeData,inputParse[2]);

							//printf("\n%s\n",inputParse[2]);

							//rec_key=atoi(inputParse[2]);

							int pure_size=strlen(inputParse[2])-1;
							//index_S[index_list_size].key=u_id;
							//[i].offset=avail_S[avail_result].off;
					/*		printf("\nkey= %d: index offset=%ld position_offset:%ld\n",index_S[index_list_size].key,index_S[index_list_size].offset,position_offset);
							printf("\nsize of writeData: %d",pure_size);
							printf("\nWritedata: %s\n",inputParse[2]);*/

							fseek(fp,position_offset,SEEK_SET);
							//printf("\npure size: %d",pure_size);
							fwrite(&pure_size,sizeof(int),1,fp);
							//printf("\nseeks to: %lu",sizeof(int)+position_offset);
							fseek(fp,(sizeof(int)+position_offset),SEEK_SET);
							fwrite(inputParse[2],pure_size,1,fp); 
							//fclose(fp);
							//fp=fopen(argv[2],"r+b");
							//fseek(fp,position_offset,SEEK_SET);
							
							//fread(&temp,sizeof(inputParse[2])+sizeof(int),1,fp);
							//printf("\n%s\n",temp);
							
							
							//free(writeData);
							fclose(fp);
							qsort(index_S,(index_list_size+1),sizeof(struct index_S),compare_index);
							/*printf("\nafter qsort:");
							for(k=0;k<=index_list_size;k++)
							{
								printf("\nindex:%d  %lu : %d",k,index_S[k].offset,index_S[k].key);
							}*/

												}

			

			}
else if(strcmp(inputParse[0],"find")==0)
{
del_key=atoi(inputParse[1]);
del_return=del_exist(del_key,index_S,&index_list_size,0);
if(del_return==-1)
	{
		printf("\nNo record with SID=%d exists",del_key);		
	}
else
	{
	fp=fopen(argv[2],"rb");
	if(!fp)
		{
			printf("\n couldnt open file");
							
		}			
	else
		{
			//printf("\nrecord at: %lu  size:%d\n",del_return,rec_siz);
			fseek(fp,del_return,SEEK_SET);
			fread(&rec_siz,sizeof(int), 1, fp);
			//printf("rec_siz of del:%d\n",rec_siz );
			fseek(fp,(sizeof(int)+del_return),SEEK_SET);
			//buf=(char *)malloc(rec_siz+1); 
			if(buf==NULL)
			{
				printf("\nmemory alloc failed");
			}
			fread(&buf,rec_siz,1,fp); 
			fclose(fp);
			buf[rec_siz] = '\0'; 
			printf("\n%s",buf);
			//free(buf);
		}


	}

}

else if(strcmp(inputParse[0],"del")==0)
{

del_key=atoi(inputParse[1]);
del_return=del_exist(del_key,index_S,&index_list_size,1);
//printf("\ndelete offset:%lu",del_return);
if(del_return==-1)
	{
		printf("\nNo record with SID=%d exists",del_key);		
	}
else
	{
	fp=fopen(argv[2],"r+b");
	if(fp==NULL)
		{
			 	//fp=fopen(argv[3],"wb");
			printf("\n couldnt open file");
				
		}			
	else
		{	//printf("")
			fseek(fp,del_return,SEEK_SET);
			fread(&rec_siz,sizeof(int), 1, fp);
			//printf("rec_siz of del:%d\n",rec_siz );
			//buf = malloc( rec_siz + 1 ); 
			//fread( buf, 1, rec_siz, fp ); 
			//buf[ rec_siz ] = '\0'; 
			//update_avail(avail_list,rec_siz+sizeof(int),del_return,&avail_list_size);
			avail_list_size++;			
			avail_S[avail_list_size].off=del_return;
			avail_S[avail_list_size].size=rec_siz+sizeof(int);
			fclose(fp);
	/*		for(k=0;k<=avail_list_size;k++)
							{
								printf("\navail:  %lu : %d",avail_S[k].off,avail_S[k].size);
							}
			for(k=0;k<=index_list_size;k++)
							{
								printf("\nindex:%d  %lu : %d",k,index_S[k].offset,index_S[k].key);
							}*/

		}
}
}

}while(1);

return 0;
}
