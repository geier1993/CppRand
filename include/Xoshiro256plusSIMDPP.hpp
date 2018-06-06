#ifndef Xoshiro256plusSIMDPP_hpp_INCLUDED
#define Xoshiro256plusSIMDPP_hpp_INCLUDED


/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include <stdint.h>
#include <array>
#include "GeneratorImplementation.hpp"

namespace PRNG {

/* This is xoshiro256+ 1.0, our best and fastest generator for floating-point
   numbers. We suggest to use its upper bits for floating-point
   generation, as it is slightly faster than xoshiro256**. It passes all
   tests we are aware of except for the lowest three bits, which might
   fail linearity tests (and just those), so if low linear complexity is
   not considered an issue (as it is usually the case) it can be used to
   generate 64-bit outputs, too.

   We suggest to use a sign test to extract a random Boolean value, and
   right shifts to extract subsets of bits.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */



struct Xoshiro256plusSIMDPP: public GeneratorImplementation<Xoshiro256plusSIMDPP,true> {
    using SV = simdpp::uint64<4>;
    using SV2 = simdpp::uint64<2>;

    using StateType = std::array<uint64_t,4>;
    using IntType   = uint64_t;
    Xoshiro256plusSIMDPP(std::array<uint64_t,4> inits): s(inits) {};

    StateType s; 

    const StateType& getState() const {
        return s;
    }

    static inline uint64_t rotl(const uint64_t x, int k) {
    	return (x << k) | (x >> (64 - k));
    }

    inline uint64_t next(void) {
    	const uint64_t result_plus = s[0] + s[3];

    	const uint64_t t = s[1] << 17;

    	using namespace simdpp;
      auto s1 = (make_int<SV2>(s[2],s[3]) ^ make_int<SV2>(s[0],s[1])).eval();
      store(s.data(),permute2<1,0>(make_int<SV2>(s[1],s[0]) ^ s1).eval());
      store(s.data()+2,s1);


    	s[2] ^= t;

    	s[3] = rotl(s[3], 45);

    	return result_plus;
    }


    /* This is the jump function for the generator. It is equivalent
       to 2^128 calls to next(); it can be used to generate 2^128
       non-overlapping subsequences for parallel computations. */
    inline void jump(void) {
    	static const uint64_t JUMP[] = { 0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c };

    	using namespace simdpp;
    	SV si = make_int<SV>(0,0,0,0);
    	for(std::size_t i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
    		for(std::size_t b = 0; b < 64; b++) {
    			if (JUMP[i] & UINT64_C(1) << b) {
    				si = (si ^ make_int<SV>(s[0],s[1],s[2],s[3])).eval();
    			}
    			next();	
    		}
    		
      store(s.data(),si);
    }
};


}

#endif // Xoshiro256plusSIMDPP_hpp_INCLUDED

