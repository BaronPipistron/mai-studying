#include <cstdlib>
#include <iostream>

const int blockAmount = 1024;
const int blockSize   = 1024;

__global__ void kernel(double* arr, std::size_t arrSize) {
    std::size_t absoluteIdx = blockIdx.x * blockDim.x + threadIdx.x;
    std::size_t offset      = blockDim.x * gridDim.x;
    
    for (std::size_t i = absoluteIdx; i < arrSize / 2; i += offset) {
        std::size_t j = arrSize - i - 1;
        double tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

int main() {
    std::cout << std::scientific;
    std::cout.precision(10);

    std::size_t arrSize;
    std::cin >> arrSize;

    if (arrSize >= (1 << 25)) {
        std::cerr << "ERROR: the size of the array is larger than the maximum allowed" << std::endl;
        return 0;
    }

    std::size_t arrSizeInBytes = sizeof(double) * arrSize;

    double* arr = static_cast<double*>(std::malloc(arrSizeInBytes));
    for (std::size_t i = 0; i != arrSize; ++i) {
        std::cin >> arr[i];
    }

    double* gpuArr;
    cudaMalloc(&gpuArr, arrSizeInBytes);
    cudaMemcpy(gpuArr, arr, arrSizeInBytes, cudaMemcpyHostToDevice);

    kernel<<<blockAmount, blockSize>>>(gpuArr, arrSize);

    cudaMemcpy(arr, gpuArr, arrSizeInBytes, cudaMemcpyDeviceToHost);

    for (std::size_t i = 0; i != arrSize; ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;

    cudaFree(gpuArr);
    std::free(arr);

    return 0;
}