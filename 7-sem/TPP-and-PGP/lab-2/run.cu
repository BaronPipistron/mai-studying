#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
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
bool readImage(const std::string& path, int32_t& w, int32_t& h, std::vector<Pixel>& pixels) {
    std::ifstream is(path, std::ios::binary);
    if (!is) { 
        std::cerr << "ERROR: can't read input file!\n"; return false;
    }

    is.read(reinterpret_cast<char*>(&w), sizeof(int32_t));
    is.read(reinterpret_cast<char*>(&h), sizeof(int32_t));

    if (w <= 0 || h <= 0 || w >= MaxValueWH || h >= MaxValueWH) {
        std::cerr << "ERROR: width or height wrong value!\n"; return false;
    }
    // static_cast to avoid overflow
    if (static_cast<int64_t>(w) * static_cast<int64_t>(h) > MaxPixels) {
        std::cerr << "ERROR: more pixels in the image than allowed!\n"; return false;
    }

    pixels.resize(w * h);
    is.read(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(Pixel));

    return true;
}

bool writeImage(const std::string& path, int32_t w, int32_t h, const std::vector<Pixel>& pixels) {
    std::ofstream os(path, std::ios::binary | std::ios::trunc);
    if (!os) {
        std::cerr << "ERROR: can't open output file!\n"; return false;
    }

    os.write(reinterpret_cast<const char*>(&w), sizeof(int32_t));
    os.write(reinterpret_cast<const char*>(&h), sizeof(int32_t));
    os.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(Pixel));

    return true;
}

// HEX file dump
inline void printByteHex(uint8_t b, std::ostream& os) {
    os << std::hex << std::setw(2) << std::setfill('0') << (unsigned)b;
    os << std::dec;
}

inline void printU32LE(uint32_t v, std::ostream& os) {
    printByteHex((uint8_t)( v        & 0xFF), os);
    printByteHex((uint8_t)((v >>  8) & 0xFF), os);
    printByteHex((uint8_t)((v >> 16) & 0xFF), os);
    printByteHex((uint8_t)((v >> 24) & 0xFF), os);
}

void dumpImageHex(int32_t w, int32_t h, const std::vector<Pixel>& pixels, std::ostream& os) {
    printU32LE((uint32_t)w, os); os << ' ';
    printU32LE((uint32_t)h, os); os << '\n';

    for (int32_t y = 0; y < h; ++y) {
        for (int32_t x = 0; x < w; ++x) {
            const Pixel& p = pixels[(size_t)y * (size_t)w + (size_t)x];
            printByteHex(p.r, os);
            printByteHex(p.g, os);
            printByteHex(p.b, os);
            printByteHex(p.a, os);
            if (x + 1 < w) os << ' ';
        }
        os << '\n';
    }
}

// Sobel kernels
__constant__ int8_t Gx[3][3]  = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
__constant__ int8_t Gy[3][3]  = { {-1,-2,-1}, { 0, 0, 0}, { 1, 2, 1} };

// Kernel function
__global__ void kernel(cudaTextureObject_t texIn, Pixel* d_out, int32_t w, int32_t h)
{
    const int xStart = blockIdx.x * blockDim.x + threadIdx.x;
    const int yStart = blockIdx.y * blockDim.y + threadIdx.y;
    const int xStep = blockDim.x * gridDim.x;
    const int yStep = blockDim.y * gridDim.y;

    for (int y = yStart; y < h; y += yStep) {
        for (int x = xStart; x < w; x += xStep) {
            float gx = 0.f;
            float gy = 0.f;

            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    float fx = (float)(x + dx) + 0.5f;
                    float fy = (float)(y + dy) + 0.5f;

                    uchar4 c = tex2D<uchar4>(texIn, fx, fy);

                    // Rec.601 яркость
                    float Y = 0.299f * (float)c.x + 0.587f * (float)c.y + 0.114f * (float)c.z;

                    int8_t kx = Gx[dy + 1][dx + 1];
                    int8_t ky = Gy[dy + 1][dx + 1];
                    gx += kx * Y;
                    gy += ky * Y;
                }
            }

            int val = (int)sqrtf(gx * gx + gy * gy);
            if (val > 255) val = 255;

            size_t i = (size_t)y * (size_t)w + (size_t)x;
            d_out[i] = Pixel{ (uint8_t)val, (uint8_t)val, (uint8_t)val, 0u };
        }
    }
}

int main() {
    std::string inFilePath, outFilePath;
    std::cin >> inFilePath >> outFilePath;

    int32_t w, h;
    std::vector<Pixel> pixels;
    if (!readImage(inFilePath, w, h, pixels)) {
        return 0;
    }

    // dumpImageHex(w, h, pixels, std::cout);

    cudaArray* arr = nullptr;
    cudaChannelFormatDesc ch = cudaCreateChannelDesc<uchar4>();
    CSC(cudaMallocArray(&arr, &ch, w, h));

    CSC(cudaMemcpy2DToArray(arr, 0, 0, pixels.data(), w * sizeof(uchar4), w * sizeof(uchar4), h, cudaMemcpyHostToDevice));

    cudaResourceDesc resDesc{};
    resDesc.resType = cudaResourceTypeArray;
    resDesc.res.array.array = arr;

    cudaTextureDesc texDesc{};
    texDesc.addressMode[0]   = cudaAddressModeClamp;
    texDesc.addressMode[1]   = cudaAddressModeClamp;
    texDesc.filterMode       = cudaFilterModePoint;
    texDesc.readMode         = cudaReadModeElementType;
    texDesc.normalizedCoords = 0;

    cudaTextureObject_t texIn = 0;
    CSC(cudaCreateTextureObject(&texIn, &resDesc, &texDesc, nullptr));

    Pixel* d_out = nullptr;
    size_t bytes = w * h * sizeof(Pixel);
    CSC(cudaMalloc(&d_out, bytes));

    kernel<<<grid, block>>>(texIn, d_out, w, h);
    CSC(cudaGetLastError());
    CSC(cudaDeviceSynchronize());

    pixels.assign(w *  h, {});
    CSC(cudaMemcpy(pixels.data(), d_out, bytes, cudaMemcpyDeviceToHost));

    if (!writeImage(outFilePath, w, h, pixels)) {
        cudaDestroyTextureObject(texIn);
        cudaFreeArray(arr);
        cudaFree(d_out);

        return 0;
    }

    cudaDestroyTextureObject(texIn);
    cudaFreeArray(arr);
    cudaFree(d_out);

    // dumpImageHex(w, h, pixels, std::cout);

    return 0;
}
