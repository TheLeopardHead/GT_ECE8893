///////////////////////////////////////////////////////////////////////////////
// Author:      <>
// Course:      ECE8893 - Parallel Programming for FPGAs
// Filename:    real_matmul.cpp
// Description: Perform matrix multiplication with real values
//
// Note:        You are free to modify this code to optimize your design.
///////////////////////////////////////////////////////////////////////////////

#include "real.h"

void real_matmul( 
    real_t MatA_DRAM[M][N], 
    real_t MatB_DRAM[N][K], 
    real_t MatC_DRAM[M][K])
{
#pragma HLS interface m_axi depth=1 port=MatA_DRAM offset=slave bundle=mem
#pragma HLS interface m_axi depth=1 port=MatB_DRAM offset=slave bundle=mem
#pragma HLS interface m_axi depth=1 port=MatC_DRAM offset=slave bundle=mem

#pragma HLS interface s_axilite port=return
    
    real_t MatA[M][N];
    real_t MatB[N][K];
    real_t MatC[M][K];

#pragma HLS array_partition variable=MatA dim=1 factor=10 cyclic
#pragma HLS array_partition variable=MatB dim=2 factor=20 cyclic
#pragma HLS array_partition variable=MatC dim=2 factor=20 cyclic
#pragma HLS array_partition variable=MatC dim=1 factor=10 cyclic

// #pragma HLS array_partition variable=MatA dim=1 complete
// #pragma HLS array_partition variable=MatB dim=2 complete
// #pragma HLS array_partition variable=MatC dim=2 complete
// #pragma HLS array_partition variable=MatC dim=1 complete


    // Read in the data (Matrix A) from DRAM to BRAM
    MAT_A_ROWS:
    for(int i = 0; i < M; i++) {
        MAT_A_COLS:
        for(int j = 0; j < N; j++) {
            MatA[i][j] = MatA_DRAM[i][j];
        }
    }

    // Read in the data (Matrix B) from DRAM to BRAM
    MAT_B_ROWS:
    for(int i = 0; i < N; i++) {
        MAT_B_COLS:
        for(int j = 0; j < K; j++) {
            MatB[i][j] = MatB_DRAM[i][j];
        }
    }

    // Initialize product matrix C
    MAT_C_ROWS_INIT:
    for(int i = 0; i < M; i++) {
        MAT_C_COLS_INIT:
        for(int j = 0; j < K; j++) {
            MatC[i][j] = 0;
        }
    }

    // Perform matrix multiplication 
    OUTER_ROWS:
    for(int i = 0; i < M; i+=10) {
        INNER_ROW_COL:
        for(int p = 0; p < N; p++) {
            OUTER_COLS:
            for(int j = 0; j < K; j+=20) {
                #pragma HLS pipeline
                for(int ii = 0; ii<10; ii++){
                    for(int jj = 0; jj < 20; jj++){
                       #pragma HLS unroll
                        MatC[i+ii][j+jj] += MatA[i+ii][p] * MatB[p][j+jj];
                    }
                }
            }
        }
    }

    // Write back the data from BRAM to DRAM
    MAT_C_ROWS:
    for(int i = 0; i < M; i++) {
        MAT_C_COLS:
        for(int j = 0; j < K; j++) {
            MatC_DRAM[i][j] = MatC[i][j];
        }
    }

}
