#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static constexpr int64_t MaxPixels = static_cast<int64_t>(4e8);
static constexpr float   kPI       = 3.14159265358979323846f;

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

// Vec3 struct
struct Vec3 {
    float x, y, z;

    __host__ __device__ Vec3() : x(0), y(0), z(0) {}
    __host__ __device__ Vec3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
};

__host__ __device__ inline Vec3 operator+(const Vec3& a, const Vec3& b) { return Vec3(a.x+b.x, a.y+b.y, a.z+b.z); }
__host__ __device__ inline Vec3 operator-(const Vec3& a, const Vec3& b) { return Vec3(a.x-b.x, a.y-b.y, a.z-b.z); }
__host__ __device__ inline Vec3 operator*(const Vec3& a, float k) { return Vec3(a.x*k, a.y*k, a.z*k); }
__host__ __device__ inline Vec3 operator/(const Vec3& a, float k) { float inv=1.0f/k; return Vec3(a.x*inv, a.y*inv, a.z*inv); }

__host__ __device__ inline float dot(const Vec3& a, const Vec3& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
__host__ __device__ inline Vec3 cross(const Vec3& a, const Vec3& b) { return Vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x); }
__host__ __device__ inline float length(const Vec3& v) { return sqrtf(dot(v,v)); }

__host__ __device__ inline Vec3 normalize(const Vec3& v) {
    float len = length(v);

    if (len < 1e-20f) {
        return Vec3(0,0,0);
    }

    return v / len;
}

__host__ __device__ inline float clamp01(float x) {
    if (x < 0.0f) {
        return 0.0f;
    }

    if (x > 1.0f) {
        return 1.0f;
    }

    return x;
}

__host__ __device__ Pixel rgbToPixel(float r, float g, float b) {
    Pixel p;

    p.r = static_cast<uint8_t>(clamp01(r)*255.0f + 0.5f);
    p.g = static_cast<uint8_t>(clamp01(g)*255.0f + 0.5f);
    p.b = static_cast<uint8_t>(clamp01(b)*255.0f + 0.5f);
    p.a = 255;

    return p;
}

__host__ __device__ inline void pixelToRGB(const Pixel& p, float& r, float& g, float& b) {
    r = static_cast<float>(p.r / 255.0f);
    g = static_cast<float>(p.g / 255.0f);
    b = static_cast<float>(p.b / 255.0f);
}

// Input format
struct FramesParams {
    int32_t amount;
    std::string outPattern;
    int32_t w, h;
    float fovDeg;
};

struct CameraParams {
    float r0c, z0c, phi0c, arc, azc, wrc, wzc, wphic, prc, pzc;
    float r0n, z0n, phi0n, arn, azn, wrn, wzn, wphin, prn, pzn;
};

struct BodyParams {
    Vec3  center;
    Pixel color;
    float radius;
    float refl;
    float transp;
    int32_t edgeLs;
};

struct FloorParams {
    Vec3 p1, p2, p3, p4;
    std::string texPath;
    Pixel color;
    float refl;
};

struct Light {
    Vec3  pos;
    Pixel color;
};

struct TailParams {
    int32_t maxDepth;
    int32_t sqrt_rpp;
};

std::istream& operator>>(std::istream& in, Vec3& v) { 
    in >> v.x >> v.y >> v.z; 
    return in; 
}

std::ostream& operator<<(std::ostream& out, const Vec3& v) { 
    out << v.x << " " << v.y << " " << v.z; 
    return out; 
}

Pixel readNormColor(std::istream& in) {
    float r,g,b; 
    in >> r >> g >> b;

    return rgbToPixel(r,g,b);
}

std::ostream& printNormColor(std::ostream& out, const Pixel& p) {
    out << (int)p.r/255.0f << " " << (int)p.g/255.0f << " " << (int)p.b/255.0f;

    return out;
}

std::istream& operator>>(std::istream& in, FramesParams& f) { 
    in >> f.amount >> f.outPattern >> f.w >> f.h >> f.fovDeg; 
    return in; 
}

std::istream& operator>>(std::istream& in, CameraParams& c) {
    in >> c.r0c >> c.z0c >> c.phi0c >> c.arc >> c.azc >> c.wrc >> c.wzc >> c.wphic >> c.prc >> c.pzc;
    in >> c.r0n >> c.z0n >> c.phi0n >> c.arn >> c.azn >> c.wrn >> c.wzn >> c.wphin >> c.prn >> c.pzn;

    return in;
}

std::istream& operator>>(std::istream& in, BodyParams& b) {
    in >> b.center;
    b.color = readNormColor(in);
    in >> b.radius >> b.refl >> b.transp >> b.edgeLs;

    return in;
}

std::istream& operator>>(std::istream& in, FloorParams& f) {
    in >> f.p1 >> f.p2 >> f.p3 >> f.p4;
    in >> f.texPath;
    f.color = readNormColor(in);
    in >> f.refl;

    return in;
}

std::istream& operator>>(std::istream& in, Light& l) {
    in >> l.pos;
    l.color = readNormColor(in);

    return in;
}

std::istream& operator>>(std::istream& in, TailParams& t) { 
    in >> t.maxDepth >> t.sqrt_rpp; 
    return in;
}

// Output .data
bool writeImage(const std::string& path, int32_t w, int32_t h, const std::vector<Pixel>& pixels) {
    std::ofstream os(path, std::ios::binary | std::ios::trunc);

    if (!os) { 
        std::cerr << "ERROR: can't open output file\n"; 
        return false; 
    }

    os.write(reinterpret_cast<const char*>(&w), sizeof(int32_t));
    os.write(reinterpret_cast<const char*>(&h), sizeof(int32_t));
    os.write(reinterpret_cast<const char*>(pixels.data()), (std::streamsize)(pixels.size() * sizeof(Pixel)));

    if (!os) { 
        std::cerr << "ERROR: can't write output\n"; 
        return false; 
    }

    return true;
}

std::string formatFramePath(const std::string& pattern, int frameIdx) {
    std::string out = pattern;
    size_t pos = out.find("%d");

    if (pos != std::string::npos) {
        out.replace(pos, 2, std::to_string(frameIdx));
        return out;
    }

    size_t dotPos = out.rfind('.');
    if (dotPos == std::string::npos) {
        return out + "_" + std::to_string(frameIdx);
    }

    out.insert(dotPos, "_" + std::to_string(frameIdx));

    return out;
}

// Triangles
struct Triangle {
    Vec3  a, b, c;
    Pixel albedo;
};

__host__ __device__ bool intersectTriangle(
    const Vec3& ro, const Vec3& rd,
    const Triangle& tri,
    float& tHit, Vec3& nHit
) {
    Vec3 e1 = tri.b - tri.a;
    Vec3 e2 = tri.c - tri.a;

    Vec3 p = cross(rd, e2);
    float det = dot(e1, p);
    if (fabsf(det) < 1e-8f) {
        return false;
    }

    float invDet = 1.0f / det;
    Vec3 t = ro - tri.a;
    float u = dot(t, p) * invDet;
    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    Vec3 q = cross(t, e1);
    float v = dot(rd, q) * invDet;
    if (v < 0.0f || (u + v) > 1.0f) {
        return false;
    }

    float tt = dot(e2, q) * invDet;
    if (tt < 1e-4f) {
        return false;
    }

    tHit = tt;
    nHit = normalize(cross(e1, e2));
    if (dot(nHit, rd) > 0.0f) {
        nHit = nHit * (-1.0f);
    }

    return true;
}

__host__ __device__ void addRayCount(uint64_t* counter, uint64_t x) {
    if (counter) {
        *counter += x;
    }
}

__host__ __device__ bool anyHitShadow(
    const Vec3& ro, const Vec3& rd, float maxT,
    const Triangle* tris, int triCount
) {
    float tHit;
    Vec3  nHit;

    for (int i = 0; i < triCount; ++i) {
        if (intersectTriangle(ro, rd, tris[i], tHit, nHit)) {
            if (tHit > 1e-4f && tHit < maxT) {
                return true;
            }
        }
    }

    return false;
}

__host__ __device__ Pixel traceRay(
    const Vec3& ro, const Vec3& rd,
    const Light& light,
    const Triangle* tris, int triCount,
    uint64_t* rayCounter
) {
    int bestIdx = -1;
    float bestT = 0.0f;
    Vec3 bestN;

    for (int i = 0; i < triCount; ++i) {
        float t;
        Vec3  n;

        if (intersectTriangle(ro, rd, tris[i], t, n)) {
            if (bestIdx < 0 || t < bestT) {
                bestIdx = i;
                bestT = t;
                bestN = n;
            }
        }
    }

    if (bestIdx < 0) {
        return Pixel{0,0,0,255};
    }

    Pixel objColor = tris[bestIdx].albedo;
    Vec3 p = ro + rd * bestT;

    Vec3 L = light.pos - p;
    float dist = length(L);

    if (dist < 1e-6f) {
        return Pixel{0,0,0,255};
    }

    Vec3 Ldir = L / dist;

    // Shadow ray
    addRayCount(rayCounter, 1);

    const float eps = 1e-3f;
    Vec3 shadowOrigin = p + bestN * eps;
    bool inShadow = anyHitShadow(shadowOrigin, Ldir, dist - eps, tris, triCount);

    float ndotl = dot(bestN, Ldir);
    if (ndotl < 0.0f) {
        ndotl = 0.0f;
    }

    float ambient = 0.18f;
    float diff    = inShadow ? 0.0f : ndotl;

    float br, bg, bb, lr, lg, lb;
    pixelToRGB(objColor, br, bg, bb);
    pixelToRGB(light.color, lr, lg, lb);

    float r = br * ambient + br * lr * diff;
    float g = bg * ambient + bg * lg * diff;
    float b = bb * ambient + bb * lb * diff;

    return rgbToPixel(r, g, b);
}

// Camera rays
__host__ __device__ void buildCameraBasis(
    const Vec3& camPos, const Vec3& camView,
    Vec3& bx, Vec3& by, Vec3& bz
) {
    bz = normalize(camView - camPos);

    Vec3 up(0.0f, 0.0f, 1.0f);
    if (fabsf(dot(bz, up)) > 0.999f) {
        up = Vec3(0.0f, 1.0f, 0.0f);
    }

    bx = normalize(cross(bz, up));
    by = normalize(cross(bx, bz));
}

__host__ __device__ Vec3 generateRayDir(
    int32_t x, int32_t y, int32_t w, int32_t h,
    float fovDeg, const Vec3& bx, const Vec3& by, const Vec3& bz
) {
    float dw = 2.0f / (float)(w - 1);
    float dh = 2.0f / (float)(h - 1);
    float z  = 1.0f / tanf(fovDeg * kPI / 360.0f);

    int32_t yf = (h - 1 - y);
    float vx = -1.0f + dw * (float)x;
    float vy = (-1.0f + dh * (float)yf) * (float)h / (float)w;

    Vec3 v(vx, vy, z);
    Vec3 dir = bx * v.x + by * v.y + bz * v.z;

    return normalize(dir);
}

// kernel
__global__ void renderKernel(
    Pixel* out,
    int32_t w, int32_t h,
    float fovDeg,
    Vec3 camPos,
    Vec3 camView,
    Light light,
    const Triangle* tris, int triCount,
    uint64_t* rayCounter
) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int offset = blockDim.x * gridDim.x;

    Vec3 bx, by, bz;
    buildCameraBasis(camPos, camView, bx, by, bz);

    int32_t total = w * h;
    for (int32_t i = idx; i < total; i += offset) {
        addRayCount(rayCounter, 1); // primary ray

        int32_t x = i % w;
        int32_t y = i / w;

        Vec3 rd = generateRayDir(x, y, w, h, fovDeg, bx, by, bz);
        out[i] = traceRay(camPos, rd, light, tris, triCount, rayCounter);
    }
}

// CPU render
static void renderCPU(
    std::vector<Pixel>& out,
    int32_t w, int32_t h,
    float fovDeg,
    const Vec3& camPos,
    const Vec3& camView,
    const Light& light,
    const Triangle* tris, int triCount,
    uint64_t& rayCounter
) {
    Vec3 bx, by, bz;
    buildCameraBasis(camPos, camView, bx, by, bz);

    for (int32_t y = 0; y < h; ++y) {
        for (int32_t x = 0; x < w; ++x) {
            rayCounter += 1; // primary ray
            Vec3 rd = generateRayDir(x, y, w, h, fovDeg, bx, by, bz);
            out[y*w + x] = traceRay(camPos, rd, light, tris, triCount, &rayCounter);
        }
    }
}

// Camera motion
static void evalCamera(const CameraParams& camera, int frame, int framesAmount, Vec3& camPos, Vec3& camView) {
    float t = 2.0f * kPI * static_cast<float>(frame) / static_cast<float>(framesAmount);

    float rc = camera.r0c + camera.arc * sinf(camera.wrc * t + camera.prc);
    float pc = camera.phi0c + camera.wphic * t;

    camPos = Vec3(
        rc * cosf(pc),
        rc * sinf(pc),
        camera.z0c + camera.azc * sinf(camera.wzc * t + camera.pzc)
    );

    float rn = camera.r0n + camera.arn * sinf(camera.wrn * t + camera.prn);
    float pn = camera.phi0n + camera.wphin * t;

    camView = Vec3(
        rn * cosf(pn),
        rn * sinf(pn),
        camera.z0n + camera.azn * sinf(camera.wzn * t + camera.pzn)
    );
}

void pushTri(std::vector<Triangle>& tris, const Vec3& a, const Vec3& b, const Vec3& c, const Pixel& col) {
    Triangle t; t.a = a; t.b = b; t.c = c; t.albedo = col;
    tris.push_back(t);
}

void addFloor(std::vector<Triangle>& tris, const FloorParams& floor) {
    pushTri(tris, floor.p1, floor.p2, floor.p3, floor.color);
    pushTri(tris, floor.p1, floor.p3, floor.p4, floor.color);
}

void addTetrahedron(std::vector<Triangle>& tris, const BodyParams& body) {
    const float invS = 1.0f / sqrtf(3.0f);
    Vec3 v[4] = {
        Vec3( 1,  1,  1) * invS,
        Vec3(-1, -1,  1) * invS,
        Vec3(-1,  1, -1) * invS,
        Vec3( 1, -1, -1) * invS
    };

    for (int i = 0; i < 4; ++i) v[i] = body.center + v[i] * body.radius;

    const int f[4][3] = {
        {0,1,2},
        {0,3,1},
        {0,2,3},
        {1,3,2}
    };

    for (int i = 0; i < 4; ++i) {
        pushTri(tris, v[f[i][0]], v[f[i][1]], v[f[i][2]], body.color);
    }
}

static void buildIcosaModel(std::vector<Vec3>& verts, std::vector<int>& faces) {
    float phi = (1.0f + sqrtf(5.0f)) * 0.5f;

    verts = {
        Vec3(-1,  phi, 0),
        Vec3( 1,  phi, 0),
        Vec3(-1, -phi, 0),
        Vec3( 1, -phi, 0),

        Vec3(0, -1,  phi),
        Vec3(0,  1,  phi),
        Vec3(0, -1, -phi),
        Vec3(0,  1, -phi),

        Vec3( phi, 0, -1),
        Vec3( phi, 0,  1),
        Vec3(-phi, 0, -1),
        Vec3(-phi, 0,  1)
    };

    for (auto& v : verts) v = normalize(v);

    int F[20][3] = {
        {0,11,5},
        {0,5,1},
        {0,1,7},
        {0,7,10},
        {0,10,11},

        {1,5,9},
        {5,11,4},
        {11,10,2},
        {10,7,6},
        {7,1,8},

        {3,9,4},
        {3,4,2},
        {3,2,6},
        {3,6,8},
        {3,8,9},

        {4,9,5},
        {2,4,11},
        {6,2,10},
        {8,6,7},
        {9,8,1}
    };

    faces.clear();
    faces.reserve(20 * 3);
    for (int i = 0; i < 20; ++i) {
        faces.push_back(F[i][0]);
        faces.push_back(F[i][1]);
        faces.push_back(F[i][2]);
    }
}

static void addIcosahedron(std::vector<Triangle>& tris, const BodyParams& body) {
    std::vector<Vec3> v;
    std::vector<int>  f;
    buildIcosaModel(v, f);

    for (size_t i = 0; i < f.size(); i += 3) {
        Vec3 a = body.center + v[f[i+0]] * body.radius;
        Vec3 b = body.center + v[f[i+1]] * body.radius;
        Vec3 c = body.center + v[f[i+2]] * body.radius;

        pushTri(tris, a, b, c, body.color);
    }
}

static void addDodecahedron(std::vector<Triangle>& tris, const BodyParams& body) {
    std::vector<Vec3> icoV;
    std::vector<int>  icoF;
    buildIcosaModel(icoV, icoF);

    const int faceCount = (int)(icoF.size() / 3);
    std::vector<Vec3> dodeV(faceCount);

    for (int fi = 0; fi < faceCount; ++fi) {
        int i0 = icoF[3*fi + 0];
        int i1 = icoF[3*fi + 1];
        int i2 = icoF[3*fi + 2];
        Vec3 c = (icoV[i0] + icoV[i1] + icoV[i2]) / 3.0f;
        dodeV[fi] = normalize(c);
    }

    for (int vi = 0; vi < icoV.size(); ++vi) {
        std::vector<int> incident;
        incident.reserve(5);

        for (int fi = 0; fi < faceCount; ++fi) {
            int i0 = icoF[3*fi + 0];
            int i1 = icoF[3*fi + 1];
            int i2 = icoF[3*fi + 2];
            if (i0 == vi || i1 == vi || i2 == vi) {
                incident.push_back(fi);
            }
        }

        if (incident.size() != 5) {
            continue;
        }

        Vec3 n = normalize(icoV[vi]);

        Vec3 up(0.0f, 0.0f, 1.0f);

        if (fabsf(dot(n, up)) > 0.999f) {
            up = Vec3(0.0f, 1.0f, 0.0f);
        }

        Vec3 b0 = normalize(cross(n, up));
        Vec3 b1 = normalize(cross(b0, n));

        struct Item { int idx; float ang; };
        Item items[5];

        for (int k = 0; k < 5; ++k) {
            Vec3 p = dodeV[incident[k]];
            Vec3 pp = p - n * dot(p, n);

            float x = dot(pp, b0);
            float y = dot(pp, b1);
            float ang = atan2f(y, x);

            items[k] = Item{incident[k], ang};
        }

        std::sort(items, items + 5, [](const Item& a, const Item& b){ return a.ang < b.ang; });

        int v0 = items[0].idx;
        for (int k = 1; k + 1 < 5; ++k) {
            int v1 = items[k].idx;
            int v2 = items[k+1].idx;

            Vec3 A = body.center + dodeV[v0] * body.radius;
            Vec3 B = body.center + dodeV[v1] * body.radius;
            Vec3 C = body.center + dodeV[v2] * body.radius;

            pushTri(tris, A, B, C, body.color);
        }
    }
}

// default
static void printDefaultConfig() {
    FramesParams frames;
    frames.amount = 256;
    frames.outPattern = "./out/img_%d.data";
    frames.w = 640;
    frames.h = 480;
    frames.fovDeg = 110.0f;

    CameraParams cam;
    cam.r0c = 7.0f; cam.z0c = 2.2f; cam.phi0c = 0.0f; cam.arc = 1.2f; cam.azc = 0.6f;
    cam.wrc = 1.0f; cam.wzc = 2.0f; cam.wphic = 1.0f; cam.prc = 0.0f; cam.pzc = 0.0f;

    cam.r0n = 0.0f; cam.z0n = 0.9f; cam.phi0n = 0.0f; cam.arn = 0.0f; cam.azn = 0.2f;
    cam.wrn = 1.2f; cam.wzn = 1.5f; cam.wphin = 0.0f; cam.prn = 0.0f; cam.pzn = 0.0f;

    BodyParams b1, b2, b3;
    b1.center = Vec3(-1.6f, 0.0f, 0.8f); b1.color = rgbToPixel(0.95f, 0.25f, 0.25f); b1.radius = 0.75f; b1.refl=0; b1.transp=0; b1.edgeLs=0;
    b2.center = Vec3( 0.0f, 0.0f, 0.9f); b2.color = rgbToPixel(0.25f, 0.95f, 0.35f); b2.radius = 0.85f; b2.refl=0; b2.transp=0; b2.edgeLs=0;
    b3.center = Vec3( 1.6f, 0.0f, 0.8f); b3.color = rgbToPixel(0.35f, 0.45f, 0.95f); b3.radius = 0.80f; b3.refl=0; b3.transp=0; b3.edgeLs=0;

    FloorParams fl;
    fl.p1 = Vec3(-6,-6,0); fl.p2 = Vec3(-6,6,0); fl.p3 = Vec3(6,6,0); fl.p4 = Vec3(6,-6,0);
    fl.texPath = "none";
    fl.color = rgbToPixel(0.25f, 0.25f, 0.28f);
    fl.refl = 0.0f;

    int lightsN = 1;
    Light l0; l0.pos = Vec3(-6.0f, 4.0f, 9.0f); l0.color = rgbToPixel(1,1,1);

    TailParams tail; tail.maxDepth = 0; tail.sqrt_rpp = 2;

    std::cout << frames.amount << "\n";
    std::cout << frames.outPattern << "\n";
    std::cout << frames.w << " " << frames.h << " " << frames.fovDeg << "\n";

    std::cout << cam.r0c << " " << cam.z0c << " " << cam.phi0c << " " << cam.arc << " " << cam.azc << " "
              << cam.wrc << " " << cam.wzc << " " << cam.wphic << " " << cam.prc << " " << cam.pzc << "\n";
    std::cout << cam.r0n << " " << cam.z0n << " " << cam.phi0n << " " << cam.arn << " " << cam.azn << " "
              << cam.wrn << " " << cam.wzn << " " << cam.wphin << " " << cam.prn << " " << cam.pzn << "\n";

    auto printBody = [](const BodyParams& b) {
        std::cout << b.center << " ";
        printNormColor(std::cout, b.color);
        std::cout << " " << b.radius << " " << b.refl << " " << b.transp << " " << b.edgeLs << "\n";
    };

    printBody(b1);
    printBody(b2);
    printBody(b3);

    std::cout << fl.p1 << " " << fl.p2 << " " << fl.p3 << " " << fl.p4 << " " << fl.texPath << " ";
    printNormColor(std::cout, fl.color);
    std::cout << " " << fl.refl << "\n";

    std::cout << lightsN << "\n";
    std::cout << l0.pos << " ";
    printNormColor(std::cout, l0.color);
    std::cout << "\n";

    std::cout << tail.maxDepth << " " << tail.sqrt_rpp << "\n";
}

int main(int argc, char** argv) {
    bool gpuFlag = false;
    bool cpuFlag = false;
    bool defaultFlag = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--gpu") gpuFlag = true;
        if (arg == "--cpu") cpuFlag = true;
        if (arg == "--default") defaultFlag = true;
    }

    if (defaultFlag) {
        printDefaultConfig();

        return 0;
    }

    bool useGPU = true;
    if (cpuFlag && !gpuFlag) {
        useGPU = false;
    }

    FramesParams frames;
    CameraParams camera;
    BodyParams b1, b2, b3;
    FloorParams floor;
    int32_t lightsN = 0;
    std::vector<Light> lights;
    TailParams tail;

    if (!(std::cin >> frames)) { 
        std::cerr << "ERROR: can't read frames params\n"; 
        return 0; 
    }

    if (!(std::cin >> camera >> b1 >> b2 >> b3 >> floor)) { 
        std::cerr << "ERROR: can't read scene params\n"; 
        return 0; 
    }

    if (!(std::cin >> lightsN)) { 
        std::cerr << "ERROR: can't read lights count\n"; 
        return 0; 
    }

    if (lightsN < 0) {
        lightsN = 0;
    }

    if (lightsN > 4) {
        lightsN = 4;
    }

    lights.resize(lightsN);
    for (int i = 0; i < lightsN; ++i) {
        if (!(std::cin >> lights[i])) { 
            std::cerr << "ERROR: can't read light params\n"; 
            return 0; 
        }
    }

    if (!(std::cin >> tail)) { 
        std::cerr << "ERROR: can't read tail params\n"; 
        return 0; 
    }

    if (frames.w <= 0 || frames.h <= 0) { 
        std::cerr << "ERROR: bad image size\n"; 
        return 0; 
    }

    if ((int64_t)frames.w * (int64_t)frames.h > MaxPixels) { 
        std::cerr << "ERROR: more pixels in the image than allowed!\n"; 
        return 0; 
    }

    if (frames.amount <= 0) { 
        std::cerr << "ERROR: bad frames amount\n"; 
        return 0; 
    }

    Light activeLight;
    if (lightsN > 0) {
        activeLight = lights[0];
    } else { 
        activeLight.pos = Vec3(-6,4,9); 
        activeLight.color = rgbToPixel(1,1,1); 
    }

    std::vector<Triangle> triHost;
    triHost.reserve(2 + 4 + 36 + 20);

    addFloor(triHost, floor);
    addTetrahedron(triHost, b1);
    addDodecahedron(triHost, b2);
    addIcosahedron(triHost, b3);

    int triCount = triHost.size();

    Triangle* tris = nullptr;
    CSC(cudaMallocManaged(&tris, sizeof(Triangle) * triCount));
    for (int i = 0; i < triCount; ++i) {
        tris[i] = triHost[i];
    }

    std::vector<Pixel> hostPixels(frames.w * frames.h);

    Pixel* d_out = nullptr;
    uint64_t* d_rays = nullptr;
    size_t bytes = frames.w * frames.h * sizeof(Pixel);

    if (useGPU) {
        CSC(cudaMalloc(&d_out, bytes));
        CSC(cudaMallocManaged(&d_rays, sizeof(uint64_t)));
    }

    for (int frame = 0; frame < frames.amount; ++frame) {
        Vec3 camPos, camView;
        evalCamera(camera, frame, frames.amount, camPos, camView);

        uint64_t rays = 0;
        auto t0 = std::chrono::high_resolution_clock::now();

        if (useGPU) {
            *d_rays = 0;

            const int kBlocks = 256;
            const int kThreads = 256;

            renderKernel<<<kBlocks, kThreads>>>(
                d_out,
                frames.w, frames.h,
                frames.fovDeg,
                camPos,
                camView,
                activeLight,
                tris, triCount,
                d_rays
            );
            CSC(cudaGetLastError());
            CSC(cudaDeviceSynchronize());

            CSC(cudaMemcpy(hostPixels.data(), d_out, bytes, cudaMemcpyDeviceToHost));

            rays = *d_rays;
        } else {
            renderCPU(hostPixels, frames.w, frames.h, frames.fovDeg, camPos, camView, activeLight, tris, triCount, rays);
        }

        auto t1 = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

        std::string outPath = formatFramePath(frames.outPattern, frame);
        (void)writeImage(outPath, frames.w, frames.h, hostPixels);

        std::cout << frame << "\t" << ms << "\t" << rays << "\n";
    }

    if (useGPU) {
        cudaFree(d_out);
        cudaFree(d_rays);
    }
    cudaFree(tris);

    return 0;
}
