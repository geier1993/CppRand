#ifndef RandomGenSIMD_hpp_INCLUDED
#define RandomGenSIMD_hpp_INCLUDED

#include "RandomGenerators.hpp"
#include "Xoshiro256starstarSIMDPP.hpp"
#include "Xoshiro256plusSIMDPP.hpp"


namespace PRNG {

template<>
struct RandomGenImplInitiator<Xoshiro256plusSIMDPP> {
    static inline Xoshiro256plusSIMDPP init(std::array<uint64_t,4> seed) {
            return Xoshiro256plusSIMDPP(seed);
        };
    static inline Xoshiro256plusSIMDPP get(std::array<uint64_t,4> seed) {
            return RandomGenImplInitiator<Xoshiro256plusSIMDPP>::init(seed);
        };
    static inline Xoshiro256plusSIMDPP __splitmixhelper(Splitmix64 sm64) {
            std::array<uint64_t,4> xorseed;
            for (unsigned int i=0; i<4;++i) {xorseed[i]=sm64.next();}
            return Xoshiro256plusSIMDPP(xorseed);
    }
    static inline Xoshiro256plusSIMDPP get(uint64_t seed) {
            return __splitmixhelper(splitmix64(seed));
        };
    static inline Xoshiro256plusSIMDPP get() {
            return __splitmixhelper(splitmix64());
        };
};
Xoshiro256plusSIMDPP xoshiro256plusSIMDPP() {
    return RandomGenImplInitiator<Xoshiro256plusSIMDPP>::get();
}
Xoshiro256plusSIMDPP xoshiro256plusSIMDPP(uint64_t seed) {
    return RandomGenImplInitiator<Xoshiro256plusSIMDPP>::get(seed);
}



template<>
struct RandomGenImplInitiator<Xoshiro256starstarSIMDPP > {
    static inline Xoshiro256starstarSIMDPP init(std::array<uint64_t,4> seed) {
            return Xoshiro256starstarSIMDPP (seed);
        };
    static inline Xoshiro256starstarSIMDPP get(std::array<uint64_t,4> seed) {
            return RandomGenImplInitiator<Xoshiro256starstarSIMDPP >::init(seed);
        };
    static inline Xoshiro256starstarSIMDPP __splitmixhelper(Splitmix64 sm64) {
            std::array<uint64_t,4> xorseed;
            for (unsigned int i=0; i<4;++i) {xorseed[i]=sm64.next();}
            return Xoshiro256starstarSIMDPP (xorseed);
    }
    static inline Xoshiro256starstarSIMDPP get(uint64_t seed) {
            return __splitmixhelper(splitmix64(seed));
        };
    static inline Xoshiro256starstarSIMDPP get() {
            return __splitmixhelper(splitmix64());
        };
};
Xoshiro256starstarSIMDPP xoshiro256starstarSIMDPP() {
    return RandomGenImplInitiator<Xoshiro256starstarSIMDPP >::get();
}
Xoshiro256starstarSIMDPP xoshiro256starstarSIMDPP(uint64_t seed) {
    return RandomGenImplInitiator<Xoshiro256starstarSIMDPP >::get(seed);
}



}

#endif // RandomGenSIMD_hpp_INCLUDED

