// 2018030328
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

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

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
    // -s 5, -E 1, -b 5 : Line 당 크기 = 32 byte -> int 8개 까지 저장 가능
    int i = 0;
    int j = 0;
    int row = 0;
    int col = 0;
    int v1, v2, v3, v4, v5, v6, v7, v8;
    
    // 32*32
    if (M == 32 && N == 32) {
        // block size = 8
        for (row = 0; row < N; row += 8){
            for (col = 0; col < M; col += 8){
                for (i = row; i < row + 8 && i < N; ++i){
                    for (j = col; j < col + 8 && j < M; ++j){
                        if (i != j) {
                            // 대각선으로 전치 -> eviction 발생 X
                            v1= A[i][j];
                            B[j][i] = v1;
                        }
                    }
                    if (row == col) {
                        // 그대로 -> eviction 일어나므로 나중에 따로 해주기
                        v1 = A[i][i];
                        B[i][i] = v1;
                    }
                }
            }
        }
    }
    /*
    // 64*64
    if (M == 64 && N == 64) {
        // block size = 4
        for (row = 0; row < N; row += 8){
            for (col = 0; col < M; col += 8){
                for(i = 0; i < 8; i++){
                    v1 = A[row+i][col];
                    v2 = A[row+i][col+1];
                    v3 = A[row+i][col+2];
                    v4 = A[row+i][col+3];
                    B[col][row+i] = v1;
                    B[col+1][row+i] = v2;
                    B[col+2][row+i] = v3;
                    B[col+3][row+i] = v4;
                }
                for(i = 0; i < 8; i++){
                    v5 = A[row+i][col+4];
                    v6 = A[row+i][col+5];
                    v7 = A[row+i][col+6];
                    v8 = A[row+i][col+7];
                    B[col+4][row+i] = v5;
                    B[col+5][row+i] = v6;
                    B[col+6][row+i] = v7;
                    B[col+7][row+i] = v8;
                }
            }
        }
    }*/
	
	if (M==64 && N==64){
		for(row = 0; row < N; row += 8){
			for(col = 0; col < M; col += 8){
				for (i = 0; i < 4; ++i){
					v1 = A[col+i][row];
					v2 = A[col+i][row+1];
					v3 = A[col+i][row+2];
					v4 = A[col+i][row+3];
					v5 = A[col+i][row+4];
					v6 = A[col+i][row+5];
					v7 = A[col+i][row+6];
					v8 = A[col+i][row+7];

					B[row][col+i] = v1;
					B[row][col+i+4] = v5;
					B[row+1][col+i] = v2;
					B[row+1][col+i+4] = v6;
					B[row+2][col+i] = v3;
					B[row+2][col+i+4] = v7;
					B[row+3][col+i] = v4;
					B[row+3][col+i+4] = v8;
				}
				for(i = 0; i < 4; ++i){
					 v1 = B[row+i][col+4];
					 v2 = B[row+i][col+5];
					 v3 = B[row+i][col+6];
					 v4 = B[row+i][col+7];

					 v5 = A[col+4][row+i];
					 v6 = A[col+5][row+i];
					 v7 = A[col+6][row+i];
					 v8 = A[col+7][row+i];

					 B[row+i][col+4] = v5;
					 B[row+i][col+5] = v6;
					 B[row+i][col+6] = v7;
					 B[row+i][col+7] = v8;

					 B[row+i+4][col] = v1;
					 B[row+i+4][col+1] = v2;
					 B[row+i+4][col+2] = v3;
					 B[row+i+4][col+3] = v4;
				}	
				for(i = 4; i < 8; ++i){
					 v5 = A[col+4][row+i];
					 v6 = A[col+5][row+i];
					 v7 = A[col+6][row+i];
					 v8 = A[col+7][row+i];

					 B[row+i][col+4] = v5;
					 B[row+i][col+5] = v6;
					 B[row+i][col+6] = v7;
					 B[row+i][col+7] = v8;
				}
			}
		}
	}
						  

    // 61*67
    if (M == 61 && N == 67) {
	// block size = 18
        for (row = 0; row < N; row += 18){
            for (col = 0; col < M; col += 18){
                for (i = row; i < row + 18 && i < N; ++i){
                    for (j = col; j < col + 18 && j < M; ++j){
                        v1= A[i][j];
                        B[j][i] = v1;
                    }
                }
            }
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

