#include "detect_objects.hpp"

using namespace cv;
using namespace cuda;

__global__ void cudaInRangeKernel(const PtrStepSz<uchar3> src, PtrStepSzb dst, int lbc0, int ubc0, int lbc1, int ubc1, int lbc2, int ubc2) {
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;

	if (x >= src.cols || y >= src.rows) return;

	uchar3 pxl = src(y, x);

	if (pxl.x >= lbc0 && pxl.x <= ubc0 && pxl.y >= lbc1 && pxl.y <= ubc1 && pxl.z >= lbc2 && pxl.z <= ubc2) {
		dst(y, x) = 255;
	} else {
		dst(y, x) = 0;
	}
}

void cudaInRange(GpuMat &src, Scalar lwr_bnd, Scalar upr_bnd, GpuMat &dst) {
	const int m = 32;
	int rows = src.rows, cols = src.cols;
	if (rows == 0 || cols == 0) return;
	
	const dim3 grid(ceil((float) cols / m), ceil((float) rows / m), 1);
	const dim3 block(m, m, 1);

	cudaInRangeKernel<<<grid, block>>>(src, dst, lwr_bnd[0], upr_bnd[0], lwr_bnd[1], upr_bnd[1], lwr_bnd[2], upr_bnd[2]);
}