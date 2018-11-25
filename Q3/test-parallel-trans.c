#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "parallel-trans.h"




int main()
{

	int M = 16;
	int N = 16;
	int i=0;
	for(;i<6;i++)
	{	
		int c=0;

		int A[N][M];
		int B[M][N];
		int i=0;	
		int j= 0;
	
		for(i=0;i<N;i++ )
			for(j=0;j<M;j++ )
				A[i][j]=i;
		printf(" ");
		simpleTrans(M,N,A,B);
		optimized_sequential(M,N,A,B);
		parallel_trans_odd_even(M,N,A,B);
		parallel_trans_2(M,N,A,B);
		parallel_trans_4(M,N,A,B);
		resetTimer();
		printf("\n\n\t\t\t***************\n\n");
		M=N=M*2;
	}
	
	return 0;
}
