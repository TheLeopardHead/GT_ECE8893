///////////////////////////////////////////////////////////////////////////////
// Author:      <>
// Course:      ECE8893 - Parallel Programming for FPGAs
// Filename:    complex_matmul.cpp
// Description: Perform matrix multiplication with complex values
//
// Note:        You are free to modify this code to implement your design.
///////////////////////////////////////////////////////////////////////////////

#include "complex.h"

void complex_matmul(
    complex_t MatA_DRAM[M][N], 
    complex_t MatB_DRAM[N][K], 
    complex_t MatC_DRAM[M][K]
)
{
#pragma HLS interface m_axi depth=1 port=MatA_DRAM offset=slave bundle=mem
#pragma HLS interface m_axi depth=1 port=MatB_DRAM offset=slave bundle=mem
#pragma HLS interface m_axi depth=1 port=MatC_DRAM offset=slave bundle=mem

#pragma HLS interface s_axilite port=return

    complex_t MatA[M][N];
    complex_t MatB[N][K]; 
    complex_t MatC[M][K];
    complex_t temp;



// #pragma HLS array_partition variable=MatA dim=1 factor=10 cyclic
// #pragma HLS array_partition variable=MatB dim=2 factor=20 cyclic
// #pragma HLS array_partition variable=MatC dim=2 factor=20 cyclic
// #pragma HLS array_partition variable=MatC dim=1 factor=10 cyclic


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
            MatC[i][j].real = 0;
            MatC[i][j].imag = 0;
        }
    }

    OUTER_ROWS:
    for(int i = 0; i < M; i++) {
        OUTER_COLS:
        for(int p = 0; p < N; p++) {
            // #pragma HLS pipeline
            temp.real = 0;
            temp.imag = 0;
            #pragma HLS pipeline II=5
            INNER_ROW_COL:
            for(int j = 0; j < K; j++) {
                int_t real_part = 0;
                int_t imag_part = 0;
                for(int jj = 0; jj < 3; jj++){
                    if(jj == 0) {
                        real_part = MatA[i][p].real * MatB[p][j].real;
                        imag_part = MatA[i][p].real * MatB[p][j].imag;
                    }
                    else if(jj == 1) {
                        real_part -= MatA[i][p].imag * MatB[p][j].imag;
                        imag_part -= MatA[i][p].imag * MatB[p][j].real;
                    }
                    else if(jj == 2) {
                        temp.real += real_part;
                        temp.imag += imag_part;
                        MatC[i][j] = temp;
                    }
                }
                // int_t real_part = MatA[i][p].real * MatB[p][j].real - MatA[i][p].imag * MatB[p][j].imag;
                // int_t imag_part = MatA[i][p].real * MatB[p][j].imag + MatA[i][p].imag * MatB[p][j].real;
                // temp.real += real_part;
                // temp.imag += imag_part;
                // MatC[i][j] = temp;
            }
        }
    }

    MAT_C_ROWS:
    for(int i = 0; i < M; i++) {
        MAT_C_COLS:
        for(int j = 0; j < K; j++) {
            MatC_DRAM[i][j] = MatC[i][j];
        }
    }

}
