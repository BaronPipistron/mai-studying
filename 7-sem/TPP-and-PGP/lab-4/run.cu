#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>
#include <thrust/device_ptr.h>
#include <thrust/extrema.h>

const dim3 block = dim3(32, 8);
const dim3 grid  = dim3(64, 64);

// Check CUDA call macros
#define CSC(call)                                                   \
do {                                                                \
    cudaError_t res = call;                                         \
    if (res != cudaSuccess) {                                       \
        fprintf(stderr, "ERROR in %s:%d. Message: %s\n",            \
                __FILE__, __LINE__, cudaGetErrorString(res));       \
        exit(0);                                                    \
    }                                                               \
} while(0)


// Comparator helper class
class Comparator {
  public:
    __host__ __device__ bool operator()(const double x, const double y) const {
        return fabs(x) < fabs(y);
    }
};

__global__ void kernelSwapRows(double *A, double *E, int n, int i, int j) {
    size_t threadsX = blockDim.x * gridDim.x;
    size_t threadsY = blockDim.y * gridDim.y;

    size_t gx       = blockIdx.x * blockDim.x + threadIdx.x;
    size_t gy       = blockIdx.y * blockDim.y + threadIdx.y;
    size_t tid      = gy * threadsX + gx;
    size_t stride   = threadsX * threadsY;

    for (size_t column = tid; column < n; column += stride) {
        // A
        double tmp = A[column * n + i];
        A[column * n + i] = A[column * n + j];
        A[column * n + j] = tmp;

        // I
        tmp = E[column * n + i];
        E[column * n + i] = E[column * n + j];
        E[column * n + j] = tmp;
    }
}

__global__ void kernelScaleRhs(const double* A, double* E, int n) {
    size_t ix = blockIdx.x * blockDim.x + threadIdx.x;
    size_t iy = blockIdx.y * blockDim.y + threadIdx.y;
    size_t sx = gridDim.x  * blockDim.x;
    size_t sy = gridDim.y  * blockDim.y;

    for (size_t row = ix; row < n; row += sx) {
        double d = A[row * n + row];

        for (size_t column = iy; column < n; column += sy) {
            E[column * n + row] /= d;
        }
    }
}

// Прямой ход - обнуление элементов под диагональю
__global__ void kernelUnderDiagonale(double* A, double* E, int n, int x)
{
    size_t ix = blockIdx.x * blockDim.x + threadIdx.x;
    size_t iy = blockIdx.y * blockDim.y + threadIdx.y;
    size_t sx = gridDim.x  * blockDim.x;
    size_t sy = gridDim.y  * blockDim.y;

    for (size_t i = x + 1 + ix; i < n; i += sx) {
        double cur = -A[x * n + i];
        double d =  A[x * n + x];
        double k =  cur / d;                                   

        for (size_t j = x + 1 + iy; j < n; j += sy) {
            A[j * n + i] += k * A[j * n + x];
        }

        for (size_t j = iy; j < n; j += sy) {
            E[j * n + i] += k * E[j * n + x];
        }
    }
}

// Обратнй ход - обнуление выыше диагонали
__global__ void kernelUpperDiagonale(const double* A, double* E, int n, int x) {
    size_t ix = blockIdx.x * blockDim.x + threadIdx.x;
    size_t iy = blockIdx.y * blockDim.y + threadIdx.y;

    size_t sx = gridDim.x  * blockDim.x;
    size_t sy = gridDim.y  * blockDim.y;

    for (int64_t i = x - 1 - ix; i >= 0; i -= sx) {
        double cur = -A[x * n + i]; 
        double d   =  A[x * n + x];
        double k   =  cur / d;

        for (int j = iy; j < n; j += sy) {
            E[j * n + i] += k * E[j * n + x];
        }
    }
}

int main() {
    int n;
    std::cin >> n;

    std::vector<double> A(n * n);
    for (size_t row = 0; row < n; ++row) {
        for (size_t column = 0; column < n; ++column) {
            std::cin >> A[column * n + row];
        }
    }

    std::vector<double> E(n * n);
    for (size_t row = 0; row < n; ++row) {
        for (size_t column = 0; column < n; ++column) {
            E[column * n + row] = ((row == column) ? 1.0 : 0.0);
        }
    }

    double *gpuA;
    double *gpuE;

    CSC(cudaMalloc(&gpuA, n * n * sizeof(double)));
    CSC(cudaMalloc(&gpuE,  n * n * sizeof(double)));
    CSC(cudaMemcpy(gpuA, A.data(), n * n * sizeof(double), cudaMemcpyHostToDevice));
    CSC(cudaMemcpy(gpuE,  E.data(),  n * n * sizeof(double), cudaMemcpyHostToDevice));

    const thrust::device_ptr<double> ptr = thrust::device_pointer_cast(gpuA);
    const Comparator comparator;

    // Прямой ход
    for (size_t i = 0; i < n - 1; ++i) {
        auto begin = ptr + i * n + i;
        auto end = ptr + i * n + n;

        auto it = thrust::max_element(thrust::device, begin, end, comparator);
        int maxElemIndex = (it - ptr) - i * n; // индекс строки

        if (maxElemIndex != i) {
            kernelSwapRows<<<grid, block>>>(gpuA, gpuE, n, i, maxElemIndex);
        }

        kernelUnderDiagonale<<<grid, block>>>(gpuA, gpuE, n, i);
    }

    // Обратный ход
    for (int64_t i = n - 1; i > 0; --i) {
        kernelUpperDiagonale<<<grid, block>>>(gpuA, gpuE, n, i);
    }

    kernelScaleRhs<<<grid, block>>>(gpuA, gpuE, n);

    CSC(cudaMemcpy(E.data(), gpuE, n * n * sizeof(double), cudaMemcpyDeviceToHost));
    CSC(cudaFree(gpuA));
    CSC(cudaFree(gpuE));

    // std::cout << std::scientific;
    // std::cout.precision(10);

    // for (size_t row = 0; row < n; ++row) {
    //     for (size_t column = 0; column < n; ++column) {
    //         if (column) {
    //             std::cout << ' ';
    //         }
    //         std::cout << E[column * n + row];
    //     }

    //     std::cout << '\n';
    // }

    return 0;
}
