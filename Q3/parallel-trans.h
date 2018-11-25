#ifndef FOO_H_   /* Include guard */
#define FOO_H_

void parallel_trans_odd_even(int M, int N, int A[N][M], int B[M][N]);

void simpleTrans(int M, int N, int A[N][M], int B[M][N]);
void parallel_trans_2(int M, int N, int A[N][M], int B[M][N]); 
void optimized_sequential(int M, int N, int A[N][M], int B[M][N]);
void parallel_trans_4(int M, int N, int A[N][M], int B[M][N]);
void resetTimer();
#endif // FOO_H_
