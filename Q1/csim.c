#include "cachelab.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "math.h"
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
struct CacheBlock {

   int32_t  tag;
   int size;
   int age;
   bool inflated ;
};



char* SplitString(char* str, char sep)
{
    char* ret = str;
    for(ret = str; *str != '\0'; ++str)
    {
        if (*str == sep)
        {
            *str = '\001';
        }
    }
    return ret;
}

int main(int argc, char *argv[] )
{

	int po =0;
	int rc = strcmp(argv[1], "-v");
	int ageCount =0;
	if(rc == 0)
		po = 1;
	

	//without parsing
	//printf( "%d\n", argc );
	double s,e,b;

	//for(v = 0 ;v<argc;v++)
		//printf("%d %s\n",v,argv[v]);
	if(po == 1)
	{
		 s = argv[3][0] - '0';
		 e = argv[5][0] - '0';
		 b = argv[7][0] - '0';
	}
	else 
	{
		 s = argv[2][0] - '0';
		 e = argv[4][0] - '0';
		 b = argv[6][0] - '0';
	
	}

	int s2 = (int)floor(s);
	
	int b2 = (int)floor(b);

	int arr[3] = {0,0,0};//zero hit, one miss, two eviction
	s = pow(2.0,s);

	b = pow(2.0,b);

	int s1 = (int)floor(s);

	int e1 = (int)floor(e);
	int b1 = (int)floor(b);

	/*printf( "Set size %d\n", s1 );
	printf( "Assiciativity %d\n", e1 );
	printf( "Block size %d\n", b1 );*/

	struct CacheBlock *** cache = malloc( sizeof( void** )*s1 );
	int i;
	
	for(i=0;i<s1;i++)
	{
		cache[i] =  malloc( sizeof( void* )*e1 );
		int j=0;
		
		for(j=0;j<e1;j++)
		{
			//
			cache[i][j] =  malloc( sizeof(struct CacheBlock ) );
			
		
		}	
	}
	
	
	for(i=0;i<s1;i++)
	{
		int j=0;
		for(j=0;j<e1;j++)
		{
			cache[i][j]->tag = -1;
			cache[i][j]->size = b;
			//cache[i][j]->age = -1;
			cache[i][j]->inflated = false;
			//printf("Tage of 0th addr on ser %d is %d\n",i,cache[i][j]->tag);
		}
			
		
	}
	//printf( "Done till here\n" );
	
	
	//open file

	FILE *fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	if(po == 1)
		fp = fopen(argv[9], "r");
	else if(po == 0 )
		fp = fopen(argv[8], "r");
	if (fp == NULL)
           exit(EXIT_FAILURE);
	int ind =-1;
       	while ((read = getline(&line, &len, fp)) != -1) 
	{

		if(line[0]!='I')
		{
		
			//printf("Token %s",line);
			//printf("Retrieved line of length %zu :\n", read);
			//char* token = strtok(line, " ");
			
			char * token1;//type of inst
			char * token2;//addrsss
			//printf("\n%c",token[0]);
			int db = 0;
			//new token method 
			char *search = " ";
			token1 = strtok(line, search);
			token2 = strtok(NULL, search);
			//printf( "Done till here\n" );
			//end
			//printf("%s  ", token1);
			char * to2;
			int m=-1;
			for(i=0;i<strlen(token2);i++)
			{
				if(token2[i] == ',')
					m=i;
			}
			to2 = malloc( m);
			strncpy(to2, token2, m );
			//printf("%s \n", to2);
       			if(token1[0] == 'M')
				db = 1;
			char c3 = token1[0];
			
			int32_t number = (int32_t)strtol(to2, NULL, 16);
			//printf("%d\n", number);
						
			int div;
			//calculate set 
			char str1[80]= {'0'};
			int c, k;
			for (c = 31; c >= 0; c--)
			{
				k = number >> c;
					 
				if (k & 1)
					strcat(str1, "1");
				else
					strcat(str1, "0");
			}
			//print binary
			//printf("\n %s \n",str1);	
			//convert numb into binary , exempt Bbits of input ,take s bits of input ,to make a set no 
			char setStr[s2+1];
			int sl = strlen(str1);
			sl = sl-(s2+b2);
			int bx =0;
			//printf("\nInfo %d  %d  \n",sl,s2);
			for(bx = sl; bx<sl+s2;bx++)
			{
				//printf(" %c ",str1[bx]);
				setStr[bx-sl] = str1[bx];
			}
			setStr[s2]= '\0';
			//printf("\n %s \n",setStr);

			//set binary string to decimal int
			int sum = (int)strtol(setStr, NULL, 2);
				
			//printf("\n %d \n",sum);
			div = sum;
			int x1 =0;
			int tv = 0;
			//int newBlock = -1;
			//look up in cache
			for(x1=0;x1<e1;x1++)
			{
				
				int td = number - cache[div][x1]->tag;
				if(td<0)
					td = -td;
				if(cache[div][x1]->inflated == false)
					tv++;
				else if (td <= b1)
				{
					
					//printf("Resultant  %" PRId32 "  ",(number - cache[div][x1]->tag));
				
						ind = 0;//hit
						cache[div][x1]->age=ageCount++;
						//cache[div][x1]->inflated=true;
						//newBlock = x1;
					
				}

					
			}

	
			//add in to the block if free e.b
			tv = 0;
			int t3 = number / b1;
				t3 = t3* b1;
			if(ind==-1)
			{
				
				
				
				//printf("\n Set%d e1%d\n ",div,e1);
				for(x1=0;x1<e1;x1++)
				{
					if(cache[div][x1]->inflated == false )
					{
						
						cache[div][x1]->tag = t3;
						cache[div][x1]->age=ageCount++;
						cache[div][x1]->inflated=true;

						ind = 1;
						break;
						
							
					}
					else 
						tv++;
					
				}
			}
				

			//check if eviction needed
			if(ind==-1)
			{
				int minAge=cache[div][0]->age;
				int indRep = 0;
				for(x1=0;x1<e1;x1++)
				{	
					//int rc = strcmp(to2, "7ff000370");
					//if(rc == 0)
					//			printf("Ages %d",cache[div][x1]->age);
					
						
					if(cache[div][x1]->inflated == true && cache[div][x1]->age <= minAge)	
					{
						indRep = x1;
						minAge = cache[div][x1]->age;
					}
				}
		//printf("\nEviction step val of indReplace: %d and div: %d\n",indRep,div);
				cache[div][indRep]->tag = t3;
				cache[div][indRep]->age = ageCount++;
				ind = 2;
			}

			if(po == 1)
				printf("%c : (Set No)%d :", c3,div);
			int x3=0;
			for(;x3<strlen(to2);x3++)
			{
				if(to2[x3]!='\n' && po == 1)
					printf("%c",to2[x3]);
			}

		
			if(ind == 0)
			{
				arr[0] = arr[0] + 1;
				if(po == 1)
				printf(" Hit ");
			}
			else if(ind == 1)
			{
				arr[1] = arr[1]+ 1;
				if(po == 1)
					printf(" Miss");
			}
			else if(ind == 2)
			{
				arr[1] = arr[1]+ 1;
				arr[2] = arr[2]+ 1;
				if(po == 1)
					printf(" Miss Eviction");
			}

			if(db)
			{
				
				arr[0] = arr[0] + 1;
				if(po == 1)
				printf(" Hit");
				db =0;

			}

			if( ind ==0 || ind ==1 || ind ==2  )
			{
				if(po == 1)
				printf("\n");
				ind = -1;
			}
			ind = -1;

		}
	}


       fclose(fp);
	
	  printSummary(arr[0], arr[1], arr[2]);
	    return 0;
}


/*void printSummary(int hits,   int misses,int evictions)
{
	printf("hits:%d misses:%d evictions:%d\n",hits,misses,evictions);

}*/
