#include "vector_add.hpp"

int main(int argc, char** argv) {
    const int SIZE = atoi(argv[1]);
    int* result = (int*) malloc(sizeof(int) * SIZE);
    for (int i = 0; i < SIZE; i++) result[i] = i;

    int* c;
    cudaMalloc((void**) &c, sizeof(int) * SIZE);

    printf("%d\n", cudaMemcpy(c, result, sizeof(int) * SIZE, cudaMemcpyHostToDevice));
    vectorAdd(c, SIZE);
    printf("%d\n", cudaMemcpy(result, c, sizeof(int) * SIZE, cudaMemcpyDeviceToHost));

    for (int i = 0; i < SIZE; i++) printf("result[%d] = %d\n", i, result[i]);

    cudaFree(c);
    delete[] result;
}