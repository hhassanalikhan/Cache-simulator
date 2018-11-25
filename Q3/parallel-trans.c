#include "parallel-trans.h"
#include <stdio.h>  
#include <pthread.h>
#include  <stdlib.h>
#include  <string.h>
#include <time.h>
#include <stdint.h>

typedef struct {
	int **A;
	int **B;
	int i;
	int M;
	int id;
	int blockSize;
	
} parm;

static unsigned long long s1 = 0;

inline uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ (
      "xorl %%eax, %%eax\n"
      "cpuid\n"
      "rdtsc\n"
      : "=a" (lo), "=d" (hi)
      :
      : "%ebx", "%ecx");
    return (uint64_t)hi << 32 | lo;
}


void *threadTranspose(void *arg)
{
	parm *p=(parm *)arg;

	//if(p->i == 1)

	int i=p->i ,j=0;
	for(  ; i < p-> M ; i = i+2 ){
		//printf("T-id: %d Index no %d\n",p->id,i);
		for( j = 0 ; j < p-> M ; j++ )
		{
			//if(p->i == 7)
				//printf(" asdas %d\n",p->A[p->i][p->j]);
			p->B[j][i] = p->A[i][j];
			
		}
	}
	return (NULL);
}


void *threadTranspose2(void *arg)
{
	parm *p=(parm *)arg;
	int i, j, ii,jj,di,temp;
	for (i 	= p->i; i < p->M; i += 16) {
		for (j = 0; j < p->M;j += p->blockSize) {
			for (ii = i; ii < i + p->blockSize; ii++) {
				di=ii;
				for (jj = j; jj < j + p->blockSize; jj++) {

					if (ii != jj)
				  		p->B[jj][ii] = p->A[ii][jj];	
					else 
						temp=p->A[ii][jj];	
				}
				if(i == j)
					p->B[di][di]=temp;
						
			}
		
		}
			
	
	 }
	return (NULL);
}

void parallel_trans_2(int M, int N, int A[N][M], int B[M][N])   /* Function definition */
{
   
	int i=0;	
	int j= 0;
	pthread_t *threads;
	int num =2;

	int **arr = (int **)malloc(N * sizeof(int *));
    		for (i=0; i<N; i++)
        		arr[i] = (int *)malloc(M * sizeof(int));
		
	for(i=0;i<N;i++ )
		for(j=0;j<M;j++ )
			arr[i][j]=A[i][j];

	int **arr2 = (int **)malloc(N * sizeof(int *));
    		for (i=0; i<N; i++)
        		arr2[i] = (int *)malloc(M * sizeof(int));
		
	for(i=0;i<N;i++ )
		for(j=0;j<M;j++ )
			arr2[i][j]=A[i][j];

	//printf("\nDone till here2\n");
	pthread_attr_t pthread_custom_attr;
	
	threads= malloc(sizeof(pthread_t)*2);
	parm *p;
	pthread_attr_init(&pthread_custom_attr);
	p= malloc(sizeof(parm)*2);
	/*for(i=0;i<N;i++ )
	{
		for(j=0;j<M;j++ )
			printf("%d ",arr2[i][j]);
		printf("\n ");
	}*/
	int ind = 0;
	printf("\n ");
	p[0].i =0;
	p[1].i = 8;
	p[0].A = arr;
	p[0].B = arr2;
	p[1].A = arr;
	p[1].B = arr2;			
	p[0].M = M;
	p[1].M = M;
	p[0].blockSize = 8;
	p[1].blockSize = 8;
	int x1 = 0;
	
	  unsigned long long x;
    unsigned long long y;
    x = rdtsc();



	for(;x1<1000;x1++)
	{
		pthread_create(&threads[0], &pthread_custom_attr, threadTranspose2, (void *)(p));
		pthread_create(&threads[1], &pthread_custom_attr, threadTranspose2, (void *)(p+1));
		pthread_join(threads[1],NULL);
		pthread_join(threads[0],NULL);
	}

  /*  for(i=0;i<N;i++ )
{
	for(j=0;j<M;j++ )
		printf("%d ",arr2[i][j]);
	printf("\n");
}*/
  
    y = rdtsc();
    y= (y-x);
    y= y/1000;
    float s2 = (s1*1.0)/y;
    printf("\nSpeedup(Multithreaded-2(avrg of 1000 executions)  %d*%d): %0.2fX ",M,M,s2);

	
}

void optimized_sequential(int M, int N, int A[N][M], int B[M][N])
{

	int i=0;	
	int j= 0;
	int blockSize = 2;
	int ii,jj;
	int di=0;
	unsigned long long x;
	unsigned long long y;
	x = rdtsc();
	for (i = 0; i < N; i += blockSize) {
		for (j = 0; j < M;j += blockSize) {
			for (ii = i; ii < i + blockSize; ii++) {
				di=ii;
				for (jj = j; jj < j + blockSize; jj++) {

					if (ii != jj)
				  		B[jj][ii] = A[ii][jj];	
					else 
					{
						B[i+blockSize][i+blockSize]=A[ii][jj];//to avoid replacing the As row with Bs row 					
					}
						
				}
				if(i == j)
					B[di][di]=B[i+blockSize][i+blockSize];
						
			}
		
		 }
			
	
	}
  
    y = rdtsc();
    y= (y-x);
   // y= y/1000;
    float s2 = (s1*1.0)/y;
    printf("\nSpeedup(Optimized sequential %d*%d): %0.2fX",M,M,s2);

}

void parallel_trans_odd_even(int M, int N, int A[N][M], int B[M][N])    /* Function definition */
{
   
	int i=0;	
	int j= 0;
	pthread_t *threads;
	int num =2;

	int **arr = (int **)malloc(N * sizeof(int *));
    		for (i=0; i<N; i++)
        		arr[i] = (int *)malloc(M * sizeof(int));
		
	for(i=0;i<N;i++ )
		for(j=0;j<M;j++ )
			arr[i][j]=A[i][j];

	int **arr2 = (int **)malloc(N * sizeof(int *));
    		for (i=0; i<N; i++)
        		arr2[i] = (int *)malloc(M * sizeof(int));
		
	for(i=0;i<N;i++ )
		for(j=0;j<M;j++ )
			arr2[i][j]=A[i][j];

	//printf("\nDone till here2\n");
	pthread_attr_t pthread_custom_attr;
	
	threads= malloc(sizeof(pthread_t)*2);
	parm *p;
	pthread_attr_init(&pthread_custom_attr);
	int firstTimeE = 0;
	int firstTimeO = 0;
	p= malloc(sizeof(parm)*2);
	/*for(i=0;i<N;i++ )
	{
		for(j=0;j<M;j++ )
			printf("%d ",arr2[i][j]);
		printf("\n ");
	}*/
	int ind = 0;
	printf("\n ");
	p[0].i =0;
	p[1].i = 1;
	p[0].id =0;
	p[1].id = 1;
	p[0].A = arr;
	p[0].B = arr2;
	p[1].A = arr;
	p[1].B = arr2;			
	p[0].M = M;
	p[1].M = M;
	int x1 = 0;
	
	  unsigned long long x;
    unsigned long long y;
    x = rdtsc();



	for(;x1<1000;x1++)
	{
		pthread_create(&threads[0], &pthread_custom_attr, threadTranspose, (void *)(p));
		pthread_create(&threads[1], &pthread_custom_attr, threadTranspose, (void *)(p+1));
		pthread_join(threads[1],NULL);
		pthread_join(threads[0],NULL);
	}

  
    y = rdtsc();
    y= (y-x);
    y= y/1000;
    //y = y/10000;
    float s2 = (s1*1.0)/y;
    printf("\nSpeedup(Multithreaded-Odd,Even(avrg of 1000 executions) %d*%d ): %0.2fX ",M,M,s2);

	
}


void simpleTrans(int M, int N, int A[N][M], int B[M][N]){
 int i, j, tmp;

    unsigned long long x;
    unsigned long long y;
    x = rdtsc();
  //  printf("X : %llu",x);
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

    y = rdtsc();
    y= (y-x);
    s1 = y;
   // printf(" Y : %llu",y);
    printf("\nSpeedup(Sequential  %d*%d): 1X ",M,M);

}



void resetTimer()
{

	s1 = 0;
}



void parallel_trans_4(int M, int N, int A[N][M], int B[M][N])
{
	int i=0;	
	int j= 0;
	pthread_t *threads;
	int num =4;

	int **arr = (int **)malloc(N * sizeof(int *));
    		for (i=0; i<N; i++)
        		arr[i] = (int *)malloc(M * sizeof(int));
		
	for(i=0;i<N;i++ )
		for(j=0;j<M;j++ )
			arr[i][j]=A[i][j];

	int **arr2 = (int **)malloc(N * sizeof(int *));
    		for (i=0; i<N; i++)
        		arr2[i] = (int *)malloc(M * sizeof(int));
		
	for(i=0;i<N;i++ )
		for(j=0;j<M;j++ )
			arr2[i][j]=A[i][j];

	//printf("\nDone till here2\n");
	pthread_attr_t pthread_custom_attr;
	
	threads= malloc(sizeof(pthread_t)*num);
	parm *p;
	pthread_attr_init(&pthread_custom_attr);
	p= malloc(sizeof(parm)*num);
	/*for(i=0;i<N;i++ )
	{
		for(j=0;j<M;j++ )
			printf("%d ",arr2[i][j]);
		printf("\n ");
	}*/
	int ind = 0;
	printf("\n ");
	p[0].i =0;
	p[1].i = 4;
	p[0].A = arr;
	p[0].B = arr2;
	p[1].A = arr;
	p[1].B = arr2;			
	p[0].M = M;
	p[1].M = M;
	p[0].blockSize = 4;
	p[1].blockSize = 4;
	p[2].i =8;
	p[3].i = 16;
	p[2].A = arr;
	p[2].B = arr2;
	p[3].A = arr;
	p[3].B = arr2;			
	p[2].M = M;
	p[3].M = M;
	p[2].blockSize = 4;
	p[3].blockSize = 4;
	int x1 = 0;
	
	  unsigned long long x;
    unsigned long long y;
    x = rdtsc();



	for(;x1<1000;x1++)
	{
		pthread_create(&threads[0], &pthread_custom_attr, threadTranspose2, (void *)(p));
		pthread_create(&threads[1], &pthread_custom_attr, threadTranspose2, (void *)(p+1));
		pthread_create(&threads[2], &pthread_custom_attr, threadTranspose2, (void *)(p+1));
		pthread_create(&threads[3], &pthread_custom_attr, threadTranspose2, (void *)(p+1));
		pthread_join(threads[1],NULL);
		pthread_join(threads[0],NULL);
		pthread_join(threads[2],NULL);
		pthread_join(threads[3],NULL);
	}

  /*  for(i=0;i<N;i++ )
{
	for(j=0;j<M;j++ )
		printf("%d ",arr2[i][j]);
	printf("\n");
}*/
  
    y = rdtsc();
    y= (y-x);
    y= y/1000;
    float s2 = (s1*1.0)/y;
    printf("\nSpeedup(Multithreaded-4(avrg of 1000 executions)  %d*%d): %0.2fX ",M,M,s2);

	
}








