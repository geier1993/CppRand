
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

using namespace PRNG;

auto vecToString = [](const auto& vec) {
        std::string s("[");
        for(auto it = vec.begin(); it!=vec.end(); ++it) {
            if(it!=vec.begin()) s.append(", ");
            s.append(std::to_string(*it));
        }
        s.append("]");
        return s;
    };

auto vecEqual = [](const auto& vec1, const auto& vec2) {
        if(vec1.size()!=vec2.size()) return false;
        for(unsigned int i=0; i<vec1.size(); ++i) {
            if(vec1[i]!=vec2[i]) return false;
        }
        return true;
    };

int main() {
    int seed= 18334;
    RandomSpacing<Xorshift1024star,true,Splitmix64> rsPersSource(seed);
    auto gen = [&](){return rsPersSource.getGenerator();};

    //auto getmovetest1 = gen();

    std::cout << "rand()\t\t" << gen().rand<int>() << std::endl;
    std::cout << "randFloat()\t" << gen().randFloat() << std::endl;
    std::cout << "randDouble()\t" << gen().randDouble() << std::endl;
    std::cout << "randInt()\t" << gen().randInt() << std::endl;
    std::cout << "randUInt()\t" << gen().randUInt() << std::endl;

    std::cout << "randInt8()\t" << (int) gen().randInt8() << std::endl;
    std::cout << "randInt16()\t" << gen().randInt16() << std::endl;
    std::cout << "randInt32()\t" << gen().randInt32() << std::endl;
    std::cout << "randInt64()\t" << gen().randInt64() << std::endl;

    std::cout << "randUInt8()\t" << (unsigned int)gen().randUInt8() << std::endl;
    std::cout << "randUInt16()\t" << gen().randUInt16() << std::endl;
    std::cout << "randUInt32()\t" << gen().randUInt32() << std::endl;
    std::cout << "randUInt64()\t" << gen().randUInt64() << std::endl;

    std::cout << "randRange(0,12)\t" << gen().randRange(0,12) << std::endl;
    std::cout << "randRange(-1.0,2.0)\t" << gen().randRange(-1.0,2.0) << std::endl;

    int a1[10];
    gen().fill(a1,10);

    int* a2 = new int[10];
    gen().fill(a2,10);

    std::vector<int> v1(10);
    gen().fill<int>(v1,10);
    assert(v1.size()==10);

    std::vector<int> v2(10);
    gen().fill<int,std::vector<int> >(v2.begin(),v2.end());
    assert(v2.size()==10);

    std::vector<int> v3 = gen().randVector<int>(10);
    assert(v3.size()==10);

    std::vector<int> v4 = gen().randVector<int,10>();
    assert(v4.size()==10);

    std::array<int,10> a3 = gen().randArray<int,10>();

    for(unsigned int i=0; i<10; ++i) {
        assert(a1[i]==a2[i]);
        assert(a2[i]==v1[i]);
        assert(v1[i]==v2[i]);
        assert(v2[i]==v3[i]);
        assert(v3[i]==v4[i]);
        assert(v4[i]==a3[i]);
    }


    RandomSpacing<Xoroshiro128plus,true,Splitmix64> rsPersSourceXoroshiro(seed);
    auto xoroshiroGen = [&](){return rsPersSourceXoroshiro.getGenerator();};
    std::cout << "rand()\t\t" << xoroshiroGen().rand<int>() << std::endl;

    RandomSpacing<Xorshift128plus,true,Splitmix64> rsPersSourceXorshift(seed);
    auto xorshiftGen = [&](){return rsPersSourceXorshift.getGenerator();};
    std::cout << "rand()\t\t" << xorshiftGen().rand<int>() << std::endl;

    RandomSpacing<Xoshiro256starstar,true,Splitmix64> rsPersSourceXoshiroStarstar(seed);
    auto xoshirostarstarGen = [&](){return rsPersSourceXoshiroStarstar.getGenerator();};
    auto xoshirostarstar = xoshirostarstarGen();
    xoshirostarstar.jump();
    std::cout << "rand()\t1\t" << xoshirostarstar.rand<int>() << std::endl;
    std::cout << "rand()\t2\t" << xoshirostarstar.rand<int>() << std::endl;
    std::cout << "rand()\t3\t" << xoshirostarstar.rand<int>() << std::endl;

    RandomSpacing<Xoshiro256plus,true,Splitmix64> rsPersSourceXoshiroPlus(seed);
    auto xoshiroplusGen = [&](){return rsPersSourceXoshiroPlus.getGenerator();};
    auto xoshiroplus = xoshiroplusGen() ;
    xoshiroplus.jump();
    std::cout << "rand()\t1\t" << xoshiroplus.rand<int>() << std::endl;
    std::cout << "rand()\t2\t" << xoshiroplus.rand<int>() << std::endl;
    std::cout << "rand()\t3\t" << xoshiroplus.rand<int>() << std::endl;

#ifdef _USE_SIMDPP
    RandomSpacing<Xoshiro256starstarSIMDPP,true,Splitmix64> rsPersSourceXoshiroStarstarSIMD(seed);
    auto xoshirostarstarSIMDGen = [&](){return rsPersSourceXoshiroStarstarSIMD.getGenerator();};
    auto xoshirostarstarSIMD = xoshirostarstarSIMDGen();
    xoshirostarstarSIMD.jump();
    std::cout << "rand()\t1\t" << xoshirostarstarSIMD.rand<int>() << std::endl;
    std::cout << "rand()\t2\t" << xoshirostarstarSIMD.rand<int>() << std::endl;
    std::cout << "rand()\t3\t" << xoshirostarstarSIMD.rand<int>() << std::endl;

    RandomSpacing<Xoshiro256plusSIMDPP,true,Splitmix64> rsPersSourceXoshiroPlusSIMD(seed);
    auto xoshiroplusSIMDPPGen = [&](){return rsPersSourceXoshiroPlusSIMD.getGenerator();};
    auto xoshiroplusSIMDPP = xoshiroplusSIMDPPGen();
    xoshiroplusSIMDPP.jump();
    std::cout << "rand()\t1\t" << xoshiroplusSIMDPP.rand<int>() << std::endl;
    std::cout << "rand()\t2\t" << xoshiroplusSIMDPP.rand<int>() << std::endl;
    std::cout << "rand()\t3\t" << xoshiroplusSIMDPP.rand<int>() << std::endl;

     //! SIMDPP usage
     using namespace simdpp;
     using VECT = int64<2>;
     std::array<uint64_t,2> bi{3,4};
     std::array<uint64_t,2> res;
    
    
     std::cout << typeid(VECT).name() << std::endl;
     VECT a = make_int<VECT>(1,2);
     VECT b = load(bi.data());
     VECT ares;
     // This does not work
     a = add(a,a).eval();
     a = add(a,a).eval();
     store(res.data(),add(a,b).eval());
     std::cout << res[0] << std::endl;
     std::cout << res[1] << std::endl;
#endif

    return(0);
}
