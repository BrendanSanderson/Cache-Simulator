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

char trans_32x64strat1[] = "32x64 done in 4x8 boxes";
void trans32x64strat1(int M, int N, int A[N][M], int B[M][N])
{
    if (N == 64 && M == 32)
    {
        for (int n = 0; n < N; n = n+8)
        {
            for (int m = 0; m < M; m = m+4)
            {
                    for(int i=n; i<n+8; i++)
                    {
                            for(int j=m; j<m+4; j++)
                            {
                                    B[j][i] = A[i][j];
                            }
                   }
            }
        }
    }
}
char trans_32strat1[] = "32x32 done in 8x8 boxes with diag check";
void trans32strat1(int M, int N, int A[N][M], int B[M][N])
{
    if (N == 32 && M == 32)
    {
        for (int n = 0; n < N; n = n+8)
        {
            for (int m = 0; m < M; m = m+8)
            {
                if (m!=n)
                {
                    for(int i=n; i<n+8; i++)
                    {
                        for(int j=m; j<m+8; j++)
                        {
                            B[j][i] = A[i][j];
                        }
                    }
                }
                else
                {
                    for(int i=n; i<n+8; i++)
                    {
                        for(int j=n; j<n+8; j++)
                        {
                            if (i != j)
                            {
                                B[j][i] = A[i][j];
                            }
                        }
                        B[i][i] = A[i][i];
                    }
                }
            }
        }
    }
}
char trans_64strat1[] = "64x64 done in 8x8 boxes with diag check";
void trans64strat1(int M, int N, int A[N][M], int B[M][N])
{
    if (M == 64 && M == 64)
    {
        for (int n = 0; n < N; n = n+4)
        {
            for (int m = 0; m < M; m = m+4)
            {
                if (m!=n)
                {
                    for(int i=n; i<n+4; i++)
                    {
                        for(int j=m; j<m+4; j++)
                        {
                            B[j][i] = A[i][j];
                        }
                    }
                }
                else
                {
                    for(int i=n; i<n+4; i++)
                    {
                        for(int j=m; j<m+4; j++)
                        {
                            if (i != j)
                            {
                                B[j][i] = A[i][j];
                            }
                        }
                        B[i][i] = A[i][i];
                    }
                }
            }
        }
    }
}
char trans_64strat2[] = "64x64 in 8x8 boxes split into 4 4x4s with diag check";
void trans64strat2(int M, int N, int A[N][M], int B[M][N])
{
    if (M == 64 && N == 64)
    {
        for (int n = 0; n < N; n = n+8)
        {
            for (int m = 0; m < M; m = m+8)
            {
                int m2 = m + 4;
                int n2 = n + 4;

                if (m!=n)
                {
                    for(int i=n; i<n+4; i++)
                    {
                        for(int j=m; j<m+4; j++)
                        {
                            B[j][i] = A[i][j];
                        }
                    }
                    for (int i = n; i < n+4; i++)
                    {
                        for(int j=m2; j<m2+4; j++)
                        {
                            B[j][i] = A[i][j];
                        }
                    }
                    for (int i = n2; i < n2+4; i++)
                    {
                        for(int j=m2; j<m2+4; j++)
                        {
                            B[j][i] = A[i][j];
                        }
                    }
                    for (int i = n2; i < n2+4; i++)
                    {
                        for(int j=m; j<m+4; j++)
                        {
                            B[j][i] = A[i][j];
                        }
                    }
                }
                else
                {
                    for(int i=n; i<n+4; i++)
                    {
                        for(int j=m; j<m+4; j++)
                        {
                            if (i != j)
                            {
                                B[j][i] = A[i][j];
                            }
                        }
                        B[i][i] = A[i][i];
                    }
                    for(int i=n; i<n+4; i++)
                    {
                        for(int j=m2; j<m2+4; j++)
                        {
                            if (i != j)
                            {
                                B[j][i] = A[i][j];
                            }
                        }
                        B[i][i] = A[i][i];
                    }
                    for(int i=n2; i<n2+4; i++)
                    {
                        for(int j=m2; j<m2+4; j++)
                        {
                            if (i != j)
                            {
                                B[j][i] = A[i][j];
                            }
                        }
                        B[i][i] = A[i][i];
                    }
                    for(int i=n2; i<n2+4; i++)
                    {
                        for(int j=m; j<m+4; j++)
                        {
                            if (i != j)
                            {
                                B[j][i] = A[i][j];
                            }
                        }
                        B[i][i] = A[i][i];
                    }
                }
            }
        }
    }
}
char trans_61strat1[] = "61x67 done with version of strat for 32x32";
void trans61strat1(int M, int N, int A[N][M], int B[M][N])
{
    if (N == 67 && M == 61)
    {
        int interval = 4;
        for (int n = 0; n < 64; n = n+interval)
        {
            for (int m = 0; m < 56; m = m+interval)
            {
                for(int i=n; i<n+interval; i++)
                {
                    for(int j=m; j<m+interval; j++)
                    {
                        B[j][i] = A[i][j];
                    }
                }
            }
            for(int i=n; i<n+interval; i++)
            {
                for(int j=56; j<M; j++)
                {
                    B[j][i] = A[i][j];
                }
            }
        }
        for (int i = 64; i < N; i++)
        {
            for (int m = 0; m < M; m = m+interval)
            {
                for(int j=m; j<m+interval; j++)
                {
                    B[j][i] = A[i][j];
                }
            }
            for(int j=56; j< M; j++)
            {
                B[j][i] = A[i][j];
            }
        }
    }      
}
char trans_61strat2[] = "61x67 done with version of 8x8 split into 4 4x4s";
void trans61strat2(int M, int N, int A[N][M], int B[M][N])
{
    if (M == 61 && N == 67)
    {
        for (int n = 0; n < 64; n = n+8)
        {
            for (int m = 0; m < 56; m = m+8)
            {
                int m2 = m + 4;
                int n2 = n + 4;

                for(int i=n; i<n+4; i++)
                {
                    for(int j=m; j<m+4; j++)
                    {
                        B[j][i] = A[i][j];
                    }
                }
                for (int i = n; i < n+4; i++)
                {
                    for(int j=m2; j<m2+4; j++)
                    {
                        B[j][i] = A[i][j];
                    }
                }
                for (int i = n2; i < n2+4; i++)
                {
                    for(int j=m2; j<m2+4; j++)
                    {
                        B[j][i] = A[i][j];
                    }
                }
                for (int i = n2; i < n2+4; i++)
                {
                    for(int j=m; j<m+4; j++)
                    {
                        B[j][i] = A[i][j];
                    }
                }
            }
            for(int i=n; i<n+4; i++)
            {
                for(int j=56; j<M; j++)
                {
                    B[j][i] = A[i][j];
                }
            }
            for(int i=n+4; i<n+8; i++)
            {
                for(int j=56; j<M; j++)
                {
                    B[j][i] = A[i][j];
                }
            }
        }
        for (int i = 64; i < N; i++)
        {
            for (int m = 0; m < M; m = m+4)
            {
                for(int j=m; j<m+4; j++)
                {
                    B[j][i] = A[i][j];
                }
            }
            for(int j=56; j< M; j++)
            {
                B[j][i] = A[i][j];
            }
        } 
    }
}
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
    if (N == 64 && M == 32)
    {
        trans32x64strat1(M,N,A,B);
    }
    else if (N == 32 && M == 32)
    {
        trans32strat1(M,N,A,B);
    }
    else if (M == 64 && N == 64)
    {
        trans64strat2(M,N,A,B);
    }
    else if (M == 61 && N == 67)
    {
        trans61strat2(M,N,A,B);
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

