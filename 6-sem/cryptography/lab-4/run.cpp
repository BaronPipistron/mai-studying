#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <tuple>
#include <vector>

struct Point {
    int64_t x;
    int64_t y;

    Point(int64_t x = 0, int64_t y = 0) : x(x), y(y) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};

class EllipticCurve {
  private:
    int64_t A, B, p;
    int64_t ap, bp;

    std::mt19937_64 rng;

  public:
    EllipticCurve(int64_t A, int64_t B, int64_t P)
        : A(A), B(B), p(P), rng(2904) {
        ap = A % p;
        bp = B % p;
    }

    int64_t getP() const { return p; }

    void setP(int64_t P) {
        p = P;
        ap = A % p;
        bp = B % p;
    }

    bool isEllipticCurve(int64_t x, int64_t y) const {
        return (powMod(y, 2, p)) == ((powMod(x, 3, p) + ap * x + bp) % p);
    }

    int64_t powMod(int64_t base, int64_t exp, int64_t mod) const {
        int64_t result = 1;
        base = base % mod;
        while (exp > 0) {
            if (exp % 2 == 1)
                result = (result * base) % mod;
            base = (base * base) % mod;
            exp /= 2;
        }
        return result;
    }

    std::vector<int64_t> extendedEuclideanAlgorithm(int64_t a, int64_t b) const {
        int64_t s = 0, t = 1, r = b;
        int64_t oldS = 1, oldT = 0, oldR = a;

        while (r != 0) {
            int64_t quotient = oldR / r;
            std::tie(oldR, r) = std::make_pair(r, oldR - quotient * r);
            std::tie(oldS, s) = std::make_pair(s, oldS - quotient * s);
            std::tie(oldT, t) = std::make_pair(t, oldT - quotient * t);
        }

        return { oldR, oldS, oldT };
    }

    int64_t inverseOf(int64_t n) const {
        auto res = extendedEuclideanAlgorithm(n, p);
        int64_t gcd = res[0], x = res[1];

        if (gcd != 1)
            return -1;
        else
            return (x % p + p) % p;
    }

    Point addPoints(const Point& p1, const Point& p2) const {
        if (p1 == Point(0, 0)) return p2;
        if (p2 == Point(0, 0)) return p1;
        if (p1.x == p2.x && p1.y != p2.y) return Point(0, 0);

        int64_t s;
        if (p1 == p2) {
            int64_t numerator = (3 * powMod(p1.x, 2, p) + ap) % p;
            int64_t denominator = inverseOf(2 * p1.y);
            if (denominator == -1) return Point(0, 0);
            s = (numerator * denominator) % p;
        } else {
            int64_t numerator = (p1.y - p2.y + p) % p;
            int64_t denominator = inverseOf((p1.x - p2.x + p) % p);
            if (denominator == -1) return Point(0, 0);
            s = (numerator * denominator) % p;
        }

        int64_t x = (powMod(s, 2, p) - 2 * p1.x + p) % p;
        int64_t y = (p1.y + s * (x - p1.x + p)) % p;

        return Point(x, (p - y) % p);
    }

    int64_t orderPoint(const Point& point) const {
        int64_t i = 1;
        Point check = addPoints(point, point);
        while (check != Point(0, 0)) {
            check = addPoints(check, point);
            ++i;
        }
        return i + 1;
    }

    int64_t step() {
        std::cout << "Curve: y^2 = x^3 + " << ap << "*x + " << bp << " mod " << p << "\n";

        std::vector<Point> points;
        auto start = std::chrono::high_resolution_clock::now();

        for (int64_t x = 0; x < p; ++x) {
            for (int64_t y = 0; y < p; ++y) {
                if (isEllipticCurve(x, y))
                    points.emplace_back(x, y);
            }
        }

        std::cout << "Curve order: " << points.size() << "\n";

        std::uniform_int_distribution<int64_t> dist(0, points.size() - 1);
        Point randomPoint = points[dist(rng)];
        std::cout << "Point " << randomPoint << " order: " << orderPoint(randomPoint) << "\n";

        auto end = std::chrono::high_resolution_clock::now();
        int64_t elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        std::cout << "Elapsed time: " << elapsedTime << " seconds\n\n";

        return elapsedTime;
    }

    bool isPrimeNumber(int64_t number) const {
        if (number < 2) return false;
        for (int64_t i = 2; i * i <= number; ++i) {
            if (number % i == 0)
                return false;
        }
        return true;
    }

    int64_t getNextPrimeNumber(int64_t start) const {
        while (!isPrimeNumber(start)) {
            ++start;
        }
        return start;
    }
};

int main() {
    int64_t a;
    int64_t b;
    int64_t p;
    int64_t timeToCalculate;

    std::cout << "Enter a:";
    std::cin >> a;
    std::cout << "Enter b:";
    std::cin >> b;
    std::cout << "Enter p:";
    std::cin >> p;
    std::cout << "Enter time in seconds:";
    std::cin >> timeToCalculate;

    std::cout << std::endl;

    EllipticCurve ec(a, b, p);

    int64_t timePassed = 0;
    int64_t iter = 1;

    while (timePassed < timeToCalculate) {
        std::cout << "Iteration " << iter << "\n";
        ec.setP(ec.getNextPrimeNumber(ec.getP() + iter * 3000));
        timePassed = ec.step();
        ++iter;
    }
    
    return 0;
}