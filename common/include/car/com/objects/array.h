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
    
template <uint8_t N> class  Array {
public:
    Array() {};
    float values[N];
    
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const Array &o ) {
        os << "["; 
        for(uint8_t i = 0; i < N; i++) os << (i==0?" ":", ") << std::setw(10) << std::setprecision(3) << o.values[i];
        os <<  "]";
        return os;
    };
#endif
};
};
};
};

#endif // CAR_COM_OBJECTS_ARRAY_H
