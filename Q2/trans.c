/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"
#include <stdbool.h>
#include<string.h>
int is_transpose(int M, int N, int A[N][M], int B[M][N]);
void trans(int M, int N, int A[N][M], int B[M][N]);
 void cachetranpose(int rb, int re, int cb, int ce,int N,int M, int A[N][M], int B[M][N]);
/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{

	int i, j, ii,jj,di;
	//int eind = 0;
	di=0;
	//int a,b,c,d,e,f,g,h = 0;
	int blockSize  ; 

	if(M==32)
	{ blockSize =8 ;
		//eind = 31;
	}
	if(M==64)
	{ blockSize =4;
		//eind = 31;
	 }
	if(M != N)
	{	
		
		trans(M,N,A,B);
	}
	else
	{
		if(M==32)    {
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

		}

	
		else

		{
			blockSize =4;/*
			//int nblockSize =16;
			//int temp;
			for (i 	= 0; i < M; i += blockSize) {
		    		for (j = 0; j < M;j += blockSize) {
					for (ii = i; ii < i + blockSize; ii++) {
						di=ii;
						for (jj = j; jj < j + blockSize; jj++) {

							if (ii != jj)
				  				B[jj][ii] = A[ii][jj];	
							else 
							{
								temp=A[ii][jj];					
							}
						
				    		}
						if(i == j)
							B[di][di]=temp;
					}
				}

						
			}*/
			
	
	
			int temp=0;
			blockSize =4;
			/*for(i = 0; i < N; ++ i)
			{
			  memcpy(&(B[i][0]), &(A[i][0]), N * sizeof(int));
			}*/
			for (i 	= 0; i < M; i += blockSize) {
		    		for (j = 0; j < M;j += blockSize) {
					for (ii = i; ii < i + blockSize; ii++) {
						di=ii;
						for (jj = j; jj < j + blockSize; jj++) {

							if (ii != jj)
				  				B[jj][ii] = A[ii][jj];	
							else 
							{
								temp=A[ii][jj];					
							}
						
				    		}
						if(i == j)
							B[di][di]=temp;
					}
				}

						
			}			
			/*printf("\n ");printf("\n ");
			for(i=0;i<N;i++ )
			{
			
				for(j=0;j<M;j++ )
					printf("%d ",B[i][j]);
				printf("\n ");
			}*/
		 

		
		}
	

	}

	
}



/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

