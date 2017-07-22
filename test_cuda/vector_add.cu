#include "vector_add.hpp"

__global__ void vectorAddKernel(int* c, int SIZE) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < SIZE)
        c[i] = 3;
}

void vectorAdd(int* c, int SIZE) {
    vectorAddKernel<<<1, SIZE>>>(c, SIZE);
}