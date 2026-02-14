#ifndef RECORD_H
#define RECORD_H

#include <cstdint>

struct Record {
    int a, x, c, z, y;
};

inline long long g(const Record& rec) {
    long long x2 = 1LL * rec.x * rec.x;
    long long c3 = 1LL * rec.c * rec.c * rec.c;
    long long z4 = 1LL * rec.z * rec.z * rec.z * rec.z;
    long long y7 = 1LL;
    for (int i = 0; i < 7; ++i) y7 *= rec.y;
    return 10LL * rec.a * (x2 + 3 * c3 * z4 - 5 * y7);
}

#endif // RECORD_H
