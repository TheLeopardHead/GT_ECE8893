///////////////////////////////////////////////////////////////////////////////
// Course:      ECE8893 - Parallel Programming for FPGAs
// Filename:    conv.h
// Description: Header file for tiling-based synthesizable implementation of
//              ResNet-50's first 7x7 convolution layer with HD input image.
//
// Note:        You are required to only modify the value of MARGIN in this
//              file. DO NOT CHANGE ANY OTHER DEFINES as this file will not
//              be included in your submission.
//
//              This file should be identical in both Part B and Part C.
///////////////////////////////////////////////////////////////////////////////

#ifndef CONV_H_
#define CONV_H_

#include <iostream>
#include <ap_fixed.h>

//--------------------------------------------------------------------------
// Type conversions for simulation and synthesis
//--------------------------------------------------------------------------
#ifdef CSIM_DEBUG
typedef float fm_t;
typedef float wt_t;
#else
typedef ap_fixed<16, 8> fm_t;
typedef ap_fixed<16, 8> wt_t;
#endif

//--------------------------------------------------------------------------
// Configuration of ResNet-50's first 7 x 7 convolution layer with HD input
//--------------------------------------------------------------------------
#define IN_FM_DEPTH 32
#define IN_FM_HEIGHT 208
#define IN_FM_WIDTH 208

#define OUT_FM_DEPTH 64
#define OUT_FM_HEIGHT 104
#define OUT_FM_WIDTH 104

#define STRIDE 2
#define PADDING 1
#define KERNEL_HEIGHT 3
#define KERNEL_WIDTH 3

//--------------------------------------------------------------------------
// Divide the input image into multiple tiles
//--------------------------------------------------------------------------
#define TILE_HEIGHT 16
#define TILE_WIDTH 16

#define N_TILE_ROWS (int)IN_FM_HEIGHT / TILE_HEIGHT
#define N_TILE_COLS (int)IN_FM_WIDTH / TILE_WIDTH

//--------------------------------------------------------------------------
// TODO: Modify the value of MARGIN based on the number of additional
//       rows and columns (belonging to adjacent tiles) required
//       to implement a functionally-correct tiling-based convolution.
//--------------------------------------------------------------------------
#define MARGIN 1

//--------------------------------------------------------------------------
// Input tile buffer dimensions
//--------------------------------------------------------------------------
#define IN_BUF_DEPTH 32
#define IN_BUF_HEIGHT TILE_HEIGHT + MARGIN
#define IN_BUF_WIDTH TILE_WIDTH + MARGIN

//--------------------------------------------------------------------------
// Output tile buffer dimensions
//--------------------------------------------------------------------------
#define OUT_BUF_DEPTH 4
#define OUT_BUF_HEIGHT TILE_HEIGHT / STRIDE
#define OUT_BUF_WIDTH TILE_WIDTH / STRIDE

//--------------------------------------------------------------------------
// Top-level Function Declaration
//--------------------------------------------------------------------------
void tiled_conv(
    fm_t input_feature_map[IN_FM_DEPTH][IN_FM_HEIGHT][IN_FM_WIDTH],
    wt_t layer_weights[OUT_FM_DEPTH][IN_FM_DEPTH][KERNEL_HEIGHT][KERNEL_WIDTH],
    wt_t layer_bias[OUT_FM_DEPTH],
    fm_t output_feature_map[OUT_FM_DEPTH][OUT_FM_HEIGHT][OUT_FM_WIDTH]);

#endif
