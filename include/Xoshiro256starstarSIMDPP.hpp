#ifndef Xoshiro256starstarSIMDPP_hpp_INCLUDED
#define Xoshiro256starstarSIMDPP_hpp_INCLUDED

/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include <stdint.h>
#include <array>
#include "GeneratorImplementation.hpp"

namespace PRNG {

/* This is xoshiro256** 1.0, our all-purpose, rock-solid generator. It has
   excellent (sub-ns) speed, a state (256 bits) that is large enough for
   any parallel application, and it passes all tests we are aware of.

   For generating just floating-point numbers, xoshiro256+ is even faster.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */

struct Xoshiro256starstarSIMDPP: public GeneratorImplementation<Xoshiro256starstarSIMDPP,true> {
    using SV = simdpp::uint64<4>;
    using SV2 = simdpp::uint64<2>;

    using StateType = std::array<uint64_t,4>;
    using IntType   = uint64_t;
    Xoshiro256starstarSIMDPP(std::array<uint64_t,4> inits): s(inits) {};

    StateType s; 

    const StateType& getState() const {
        return s;
    }

    static inline uint64_t rotl(const uint64_t x, int k) {
    	return (x << k) | (x >> (64 - k));
    }

    inline uint64_t next(void) {
    	const uint64_t result_starstar = rotl(s[1] * 5, 7) * 9;

    	const uint64_t t = s[1] << 17;

    	using namespace simdpp;

      auto s1 = (make_int<SV2>(s[2],s[3]) ^ make_int<SV2>(s[0],s[1])).eval();
      store(s.data(),permute2<1,0>(make_int<SV2>(s[1],s[0]) ^ s1).eval());
      store(s.data()+2,s1);

    	s[2] ^= t;

    	s[3] = rotl(s[3], 45);

    	return result_starstar;
    }


    /* This is the jump function for the generator. It is equivalent
       to 2^128 calls to next(); it can be used to generate 2^128
       non-overlapping subsequences for parallel computations. */
    inline void jump(void) {
    	static const uint64_t JUMP[] = { 0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c };

    	using namespace simdpp;
    	SV si = make_int<SV>(0,0,0,0);
    	SV sires;
    	for(std::size_t i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
    		for(std::size_t b = 0; b < 64; b++) {
    			if (JUMP[i] & UINT64_C(1) << b) {
    				sires = (si ^ make_int<SV>(s[0],s[1],s[2],s[3])).eval();
    				si = sires;
    			}
    			next();	
    		}

      store(s.data(),si);
    }
};


};

#endif // Xoshiro256starstarSIMDPP_hpp_INCLUDED

