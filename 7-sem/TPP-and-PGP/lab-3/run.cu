#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

const int MaxPixels  = 4 * 1e8;
const int MaxClasses = 32;

// Check CUDA call macros
#define CSC(call)                                                      \
do {                                                                   \
    cudaError_t res = call;                                            \
    if (res != cudaSuccess) {                                          \
        fprintf(stderr, "ERROR in %s:%d. Message: %s\n",               \
                __FILE__, __LINE__, cudaGetErrorString(res));          \
        exit(0);                                                       \
    }                                                                  \
} while (0)

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
        std::cerr << "ERROR: can't read input file!\n";
        return false;
    }

    is.read(reinterpret_cast<char*>(&w), sizeof(int32_t));
    is.read(reinterpret_cast<char*>(&h), sizeof(int32_t));

    if (static_cast<int64_t>(w) * static_cast<int64_t>(h) > MaxPixels) {
        std::cerr << "ERROR: more pixels in the image than allowed!\n";
        return false;
    }

    pixels.resize(static_cast<size_t>(w) * static_cast<size_t>(h));
    is.read(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(Pixel));

    if (!is) {
        std::cerr << "ERROR: can't read pixels!\n";
        return false;
    }

    return true;
}

bool writeImage(const std::string& path, int32_t w, int32_t h, const std::vector<Pixel>& pixels) {
    std::ofstream os(path, std::ios::binary | std::ios::trunc);
    if (!os) {
        std::cerr << "ERROR: can't open output file!\n";
        return false;
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

// в константу кладём уже нормализованные средние
// храним подряд: [c0.r, c0.g, c0.b, c1.r, c1.g, c1.b, и тд]
__constant__ float c_classVec[MaxClasses * 3];

// kernel function
__global__ void kernel(const Pixel* d_in, Pixel* d_out,
                       int32_t totalPixels, int32_t nc)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int offset = blockDim.x * gridDim.x;

    for (int i = idx; i < totalPixels; i += offset) {
        Pixel p = d_in[i];

        float pr = static_cast<float>(p.r); 
        float pg = static_cast<float>(p.g); 
        float pb = static_cast<float>(p.b);

        int   bestClass = 0;
        float bestScore =
            pr * c_classVec[0 * 3 + 0] +
            pg * c_classVec[0 * 3 + 1] +
            pb * c_classVec[0 * 3 + 2];

        for (int c = 1; c < nc; ++c) {
            float score =
                pr * c_classVec[c * 3 + 0] +
                pg * c_classVec[c * 3 + 1] +
                pb * c_classVec[c * 3 + 2];

            if (score > bestScore) {
                bestScore = score;
                bestClass = c;
            }
        }

        Pixel q = p;
        q.a = static_cast<uint8_t>(bestClass);
        d_out[i] = q;
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

    int nc = 0;
    if (!(std::cin >> nc)) {
        std::cerr << "ERROR: can't read number of classes\n";
        return 0;
    }

    if (nc <= 0 || nc > MaxClasses) {
        std::cerr << "ERROR: bad number of classes\n";
        return 0;
    }

    float h_means[MaxClasses][3] = {0.0f};

    for (int j = 0; j < nc; ++j) {
        int npj = 0;
        std::cin >> npj;

        if (npj <= 0) {
            std::cerr << "ERROR: zero samples for class " << j << "\n";
            return 0;
        }

        double sumR = 0.0;
        double sumG = 0.0;
        double sumB = 0.0;

        for (int i = 0; i < npj; ++i) {
            int x, y;
            std::cin >> x >> y;

            if (x < 0 || x >= w || y < 0 || y >= h) {
                std::cerr << "ERROR: bad sample coords for class " << j << "\n";
                return 0;
            }
            
            const Pixel& p = pixels[y * w + x];
            sumR += static_cast<double>(p.r);
            sumG += static_cast<double>(p.g);
            sumB += static_cast<double>(p.b);
        }

        double invN = 1.0 / static_cast<double>(npj);
        h_means[j][0] = static_cast<float>(sumR * invN);
        h_means[j][1] = static_cast<float>(sumG * invN);
        h_means[j][2] = static_cast<float>(sumB * invN);
    }

    float h_normed[MaxClasses * 3];
    for (int j = 0; j < nc; ++j) {
        float r = h_means[j][0];
        float g = h_means[j][1];
        float b = h_means[j][2];
        float len = std::sqrt(r * r + g * g + b * b);

        if (len < 1e-6f) {
            h_normed[j * 3 + 0] = 1.0f;
            h_normed[j * 3 + 1] = 0.0f;
            h_normed[j * 3 + 2] = 0.0f;
        } else {
            h_normed[j * 3 + 0] = r / len;
            h_normed[j * 3 + 1] = g / len;
            h_normed[j * 3 + 2] = b / len;
        }
    }

    CSC(cudaMemcpyToSymbol(c_classVec, h_normed, sizeof(float) * MaxClasses * 3, 0, cudaMemcpyHostToDevice));

    Pixel* d_in  = nullptr;
    Pixel* d_out = nullptr;
    size_t bytes = w * h * sizeof(Pixel);

    CSC(cudaMalloc(&d_in, bytes));
    CSC(cudaMalloc(&d_out, bytes));

    CSC(cudaMemcpy(d_in, pixels.data(), bytes, cudaMemcpyHostToDevice));

    int32_t totalPixels = w * h;
    kernel<<<1024, 1024>>>(d_in, d_out, totalPixels, nc);
    CSC(cudaGetLastError());
    CSC(cudaDeviceSynchronize());

    pixels.assign(totalPixels, {});
    CSC(cudaMemcpy(pixels.data(), d_out, bytes, cudaMemcpyDeviceToHost));

    if (!writeImage(outFilePath, w, h, pixels)) {
        cudaFree(d_in);
        cudaFree(d_out);

        return 0;
    }

    cudaFree(d_in);
    cudaFree(d_out);

    // dumpImageHex(w, h, pixels, std::cout);

    return 0;
}
