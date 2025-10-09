#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const int MaxValueWH = 1 << 16;
const int MaxPixels  = 1e+8;

const dim3 block = dim3(32, 8);
const dim3 grid  = dim3(64, 64);

// Check CUDA call macros
#define CSC(call)  									                \
do {											                    \
	cudaError_t res = call;							                \
	if (res != cudaSuccess) {							            \
		fprintf(stderr, "ERROR in %s:%d. Message: %s\n",			\
				__FILE__, __LINE__, cudaGetErrorString(res));		\
		exit(0);								                    \
	}										                        \
} while(0)

// Pixel struct
struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

// Helper functions (host)
bool readImage(const std::string& path, 
               int32_t& w, int32_t& h,
               std::vector<Pixel>& pixels) 
{
    std::ifstream is(path, std::ios::binary);
    if (!is) {
        std::cerr << "ERROR: can't read input file!" << std::endl;
        return false;
    }

    int32_t wReaded;
    int32_t hReaded;

    is.read(reinterpret_cast<char*>(&wReaded), sizeof(int32_t));
    is.read(reinterpret_cast<char*>(&hReaded), sizeof(int32_t));

    w = wReaded;
    h = hReaded;

    if (w <= 0 || h <= 0 || w >= MaxValueWH || h >= MaxValueWH) {
        std::cerr << "ERROR: width or height wrong value!" << std::endl;
        return false;
    }

    if (w * h > MaxPixels) {
        std::cerr << "ERROR: more pixels in the image than allowed!" << std::endl;
        return false;
    }

    pixels.resize(w * h);

    for (std::size_t i = 0; i != pixels.size(); ++i) {
        is.read(reinterpret_cast<char*>(&pixels[i]), sizeof(Pixel));
    }

    return true;
}

bool writeImage(const std::string& path, 
                int32_t w, int32_t h,
                const std::vector<Pixel>& pixels)
{
    std::ofstream os(path, std::ios::binary | std::ios::trunc);
    if (!os) {
        std::cerr << "ERROR: can't open output file!" << std::endl;
        return false;
    }

    os.write(reinterpret_cast<char*>(&w), sizeof(int32_t));
    os.write(reinterpret_cast<char*>(&h), sizeof(int32_t));

    for (std::size_t i = 0; i != pixels.size(); ++i) {
        Pixel pixelToWrite = pixels[i];
        os.write(reinterpret_cast<char*>(&pixelToWrite), sizeof(Pixel));
    }

    return true;
}

// Sobel kernels
__constant__ int8_t Gx[3][3]  = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

__constant__ int8_t Gy[3][3] = {
    {-1, -2, -1},
    {0, 0, 0},
    {1, 2, 1}
};

// Device function
__global__ void kernel(Pixel* d_in, Pixel* d_out, int32_t w, int32_t h) {
    const int x_start = blockIdx.x * blockDim.x + threadIdx.x;
    const int y_start = blockIdx.y * blockDim.y + threadIdx.y;
    
    const int xStep = blockDim.x * gridDim.x;
    const int yStep = blockDim.y * gridDim.y;

    for (int y = y_start; y < h; y += yStep) {
        for (int x = x_start; x < w; x += xStep) {
            float gx = 0.f;
            float gy = 0.f;

            for (int dy = -1; dy <= 1; ++dy) {
                int32_t yy = max(0, min(y + dy, h - 1));

                for (int dx = -1; dx <= 1; ++dx) {
                    int32_t xx = max(0, min(x + dx, w - 1));

                    Pixel p = d_in[yy * w + xx];

                    // Rec.601 consts
                    float Y = 0.299f * static_cast<float>(p.r) + \
                              0.587f * static_cast<float>(p.g) + \
                              0.114f * static_cast<float>(p.b);

                    int8_t kx = Gx[dy + 1][dx + 1];
                    int8_t ky = Gy[dy + 1][dx + 1];

                    gx += kx * Y;
                    gy += ky * Y;
                }
            }

            float mag = sqrtf(gx * gx + gy * gy);
            int val = static_cast<int32_t>(mag);

            if (val > 255) {
                val = 255;
            }

            std::size_t i = y * w + x;
            d_out[i] = Pixel{ uint8_t(val), uint8_t(val), uint8_t(val), 0u};
        }
    }
}

int main() {
    std::string inFilePath;
    std::string outFilePath;

    std::cin >> inFilePath;
    std::cin >> outFilePath;

    // Initialized in readImage func
    int32_t w;
    int32_t h;

    std::vector<Pixel> pixels;
    
    if (!readImage(inFilePath, w, h, pixels)) {
        return 0;
    }

    std::size_t bytes = sizeof(Pixel) * pixels.size();

    Pixel* devicePixelsIn;
    Pixel* devicePixelsOut;
    
    CSC(cudaMalloc(&devicePixelsIn, bytes));
    CSC(cudaMalloc(&devicePixelsOut, bytes));
    
    CSC(cudaMemcpy(devicePixelsIn, pixels.data(), bytes, cudaMemcpyHostToDevice));
    
    kernel<<<grid, block>>>(devicePixelsIn, devicePixelsOut, w, h);

    // Clear input pixels and copy updated from device in one vector
    pixels.clear();
    pixels.resize(w * h);
    CSC(cudaMemcpy(pixels.data(), devicePixelsOut, bytes, cudaMemcpyDeviceToHost));

    if (!writeImage(outFilePath, w, h, pixels)) {
        return 0;
    }

    cudaFree(devicePixelsIn);
    cudaFree(devicePixelsOut);

    return 0;
}
