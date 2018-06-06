
#include "RandomGenerators.hpp" 

#ifdef _USE_SIMDPP
#define SIMDPP_ARCH_X86_SSE2 	
#define SIMDPP_ARCH_X86_SSE3 	
#define SIMDPP_ARCH_X86_SSSE3 
#define SIMDPP_ARCH_X86_SSE4_1

#include "simdpp/simd.h"
#include "RandomGeneratorsSIMD.hpp" 
#endif

#include <iostream>
#include <string>
#include <assert.h>
#include <typeinfo>
#include <chrono>

using namespace PRNG;


int main() {
    int seed= 18334;

    std::size_t iterations = 2<<20;

#ifdef _USE_SIMDPP
    SequenceSplitting<Xoshiro256starstar,true,Splitmix64>       ssXoshiro256starstar(seed);
    SequenceSplitting<Xoshiro256starstarSIMDPP,true,Splitmix64> ssXoshiro256starstarSIMD(seed);
    SequenceSplitting<Xoshiro256plus,true,Splitmix64>           ssXoshiro256plus(seed);
    SequenceSplitting<Xoshiro256plusSIMDPP,true,Splitmix64>     ssXoshiro256plusSIMD(seed);

    auto  xoshiro256starstar     = ssXoshiro256starstar.getGenerator();
    auto  xoshiro256starstarSIMD = ssXoshiro256starstarSIMD.getGenerator();
    auto  xoshiro256plus         = ssXoshiro256plus.getGenerator();
    auto  xoshiro256plusSIMD     = ssXoshiro256plusSIMD.getGenerator();

    auto runTest = [&](auto& gen) {
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();

        int res=0;
        for(std::size_t i=0; i < iterations; ++i) {
          res = gen.template rand<int>();
        }

        end = std::chrono::system_clock::now();
 
        int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
                             (end-start).count();
 
        std::cout << elapsed_seconds << "s\n";

        return res;
    };

    std::cout << "xoshiro256starstar" << std::endl;
    std::cout << runTest(xoshiro256starstar     ) << std::endl;
    std::cout << "xoshiro256starstarSIMD"  << std::endl;
    std::cout << runTest(xoshiro256starstarSIMD ) << std::endl;
    std::cout << "xoshiro256plus" << std::endl;
    std::cout << runTest(xoshiro256plus         ) << std::endl;
    std::cout << "xoshiro256plusSIMD"     << std::endl;
    std::cout << runTest(xoshiro256plusSIMD     ) << std::endl;



    // //! SIMDPP usage
    // using namespace simdpp;
    // using VECT = int64<2>;
    // std::array<uint64_t,2> bi{3,4};
    // std::array<uint64_t,2> res;
    //
    //
    // std::cout << typeid(VECT).name() << std::endl;
    // VECT a = make_int<VECT>(1,2);
    // VECT b = load(bi.data());
    // a = add(a,a).eval();
    // store(res.data(),add(a,b).eval());
    // std::cout << res[0] << std::endl;
    // std::cout << res[1] << std::endl;
#endif

    return(0);
}
