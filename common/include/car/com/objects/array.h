#ifndef CAR_COM_OBJECTS_ARRAY_H
#define CAR_COM_OBJECTS_ARRAY_H

#include <cstdint>

#if defined(__amd64__)
#include <iostream>
#include <iomanip>      // std::setprecision
#endif

namespace car {
namespace com {
namespace objects {

template <class T, uint8_t N> class Array {
public:
    Array() {};
    T values[N];
    size_t size() {
        return N;
    }
    T &operator[](size_t n) {
        return values[n];
    }
    const T &operator[](size_t n) const {
        return values[n];
    }
    void clear() {
        for(size_t i = 0; i < N; i++) values[i] = 0;
    }
    void fill(T v) {
        for(size_t i = 0; i < N; i++) values[i] = v;
    }



#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const Array &o ) {
        os << "[";
        for(uint8_t i = 0; i < N; i++) os << (i==0?" ":", ") << std::setw(10) << std::setprecision(3) << o.values[i];
        os <<  "]";
        return os;
    };
#endif
};

typedef Array<float, 4> Array16FC4;
typedef Array<float, 8> Array16FC8;
typedef Array<int16_t, 1> Array16SC1;
typedef Array<int16_t, 4> Array16SC4;
typedef Array<int16_t, 8> Array16SC8;
};
};
};

#endif // CAR_COM_OBJECTS_ARRAY_H
