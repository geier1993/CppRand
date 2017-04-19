#ifndef RandomGen_hpp_INCLUDED
#define RandomGen_hpp_INCLUDED

#include <limits>
#include <type_traits>
#include <stdint.h>
#include <iterator>
#include <vector>
#include <array>
#include "GeneratorImplementation.hpp"
#include "Splitmix64.hpp"
#include "Xorshift1024star.hpp"
#include "Xorshift128plus.hpp"
#include "Xoroshiro128plus.hpp"


namespace PRNG {

// TODO static implementation (i.e. source splitting with template indices...)
// TODO perservative - non-perservative sources
// TODO sources with more generator implementations 


/*
 * Initiator - used to initate a Generator implementation with different kind of initial states.
 */
template<typename Generator>
struct RandomGenImplInitiator {};

template<>
struct RandomGenImplInitiator<Splitmix64> {
    static inline Splitmix64 init(uint64_t seed) {return Splitmix64(seed);};
    static inline Splitmix64 get(uint64_t seed) {return RandomGenImplInitiator<Splitmix64>::init(seed);};
    static inline Splitmix64 get() {return RandomGenImplInitiator<Splitmix64>::get(rand());};
};
Splitmix64 splitmix64() {
    return RandomGenImplInitiator<Splitmix64>::get();
}
Splitmix64 splitmix64(uint64_t seed) {
    return RandomGenImplInitiator<Splitmix64>::get(seed);
}

template<>
struct RandomGenImplInitiator<Xorshift1024star> {
    static inline Xorshift1024star init(std::array<uint64_t,16> seed) {
            return Xorshift1024star(seed);
        };
    static inline Xorshift1024star get(std::array<uint64_t,16> seed) {
            return RandomGenImplInitiator<Xorshift1024star>::init(seed);
        };
    static inline Xorshift1024star __splitmixhelper(Splitmix64 sm64) {
            std::array<uint64_t,16> xorseed;
            for (unsigned int i=0; i<16;++i) {xorseed[i]=sm64.next();}
            return Xorshift1024star(xorseed);
    }
    static inline Xorshift1024star get(uint64_t seed) {
            return __splitmixhelper(splitmix64(seed));
        };
    static inline Xorshift1024star get() {
            return __splitmixhelper(splitmix64());
        };
};
Xorshift1024star xorshift1024star() {
    return RandomGenImplInitiator<Xorshift1024star>::get();
}
Xorshift1024star xorshift1024star(uint64_t seed) {
    return RandomGenImplInitiator<Xorshift1024star>::get(seed);
}



template<>
struct RandomGenImplInitiator<Xorshift128plus> {
    static inline Xorshift128plus init(std::array<uint64_t,2> seed) {
            return Xorshift128plus(seed);
        };
    static inline Xorshift128plus get(std::array<uint64_t,2> seed) {
            return RandomGenImplInitiator<Xorshift128plus>::init(seed);
        };
    static inline Xorshift128plus __splitmixhelper(Splitmix64 sm64) {
            std::array<uint64_t,2> xorseed;
            for (unsigned int i=0; i<2;++i) {xorseed[i]=sm64.next();}
            return Xorshift128plus(xorseed);
    }
    static inline Xorshift128plus get(uint64_t seed) {
            return __splitmixhelper(splitmix64(seed));
        };
    static inline Xorshift128plus get() {
            return __splitmixhelper(splitmix64());
        };
};
Xorshift128plus xorshift128plus() {
    return RandomGenImplInitiator<Xorshift128plus>::get();
}
Xorshift128plus xorshift128plus(uint64_t seed) {
    return RandomGenImplInitiator<Xorshift128plus>::get(seed);
}

template<>
struct RandomGenImplInitiator<Xoroshiro128plus> {
    static inline Xoroshiro128plus init(std::array<uint64_t,2> seed) {
            return Xoroshiro128plus(seed);
        };
    static inline Xoroshiro128plus get(std::array<uint64_t,2> seed) {
            return RandomGenImplInitiator<Xoroshiro128plus>::init(seed);
        };
    static inline Xoroshiro128plus __splitmixhelper(Splitmix64 sm64) {
            std::array<uint64_t,2> xorseed;
            for (unsigned int i=0; i<2;++i) {xorseed[i]=sm64.next();}
            return Xoroshiro128plus(xorseed);
    }
    static inline Xoroshiro128plus get(uint64_t seed) {
            return __splitmixhelper(splitmix64(seed));
        };
    static inline Xoroshiro128plus get() {
            return __splitmixhelper(splitmix64());
        };
};
Xoroshiro128plus xoroshiro128plus() {
    return RandomGenImplInitiator<Xoroshiro128plus>::get();
}
Xoroshiro128plus xoroshiro128plus(uint64_t seed) {
    return RandomGenImplInitiator<Xoroshiro128plus>::get(seed);
}



/*
 * Wrapper around GeneratorImplementation with supporting functions
 */
template<typename GeneratorImpl>
struct RandomGenerator: GeneratorImpl {
    using Inttype = typename GeneratorImpl::IntType;
    inline Inttype max() const { return std::numeric_limits<Inttype>::max();};

    RandomGenerator(RandomGenerator<GeneratorImpl>&& initiated): GeneratorImpl(std::move(initiated)) {};
    RandomGenerator(GeneratorImpl&& initiated): GeneratorImpl(std::move(initiated)) {};

    auto& operator=(RandomGenerator<GeneratorImpl>&& moveass) {
        GeneratorImpl::operator=(std::move(moveass));
        return *this;
    }
    auto& operator=(const RandomGenerator<GeneratorImpl>& ass) {
        GeneratorImpl::operator=(ass);
        return *this;
    }

    template<typename ...T>
    RandomGenerator(T&& ... args): GeneratorImpl(RandomGenImplInitiator<GeneratorImpl>::get(std::forward<T>(args)...)) {}

    template<typename T,
        typename std::enable_if<std::is_floating_point<T>::value,int>::type=0 >
    T       rand        ()  { return ((T) GeneratorImpl::next())/((T)max());}
    float   randFloat   ()  { return rand<float>();};
    double  randDouble  ()  { return rand<double>();};

    // TODO, export rand specializations in extern class such that they have to be implemented per Implementation
    template<typename T,
        typename std::enable_if<std::is_integral<T>::value,int>::type=0 >
    T        rand       ()  { return GeneratorImpl::next();          }
    int      randInt    ()  { return rand<int>();     }
    unsigned int randUInt    ()  { return rand<unsigned int>();     }
    long int      randLInt    ()  { return rand<long int>();     }
    long unsigned int randULInt    ()  { return rand<long unsigned int>();     }
    int8_t   randInt8   ()  { return rand<int8_t>();  }
    int16_t  randInt16  ()  { return rand<int16_t>(); }
    int32_t  randInt32  ()  { return rand<int32_t>(); }
    int64_t  randInt64  ()  { return rand<int64_t>(); }
    uint8_t  randUInt8  ()  { return rand<uint8_t>(); }
    uint16_t randUInt16 ()  { return rand<uint16_t>();}
    uint32_t randUInt32 ()  { return rand<uint32_t>();}
    uint64_t randUInt64 ()  { return rand<uint64_t>();}

    template<typename T>
    void fill(T* u, unsigned int size) {
        for(unsigned int i=0; i<size;++i) {
            u[i]=rand<T>();
        }
    }
    template<typename T, typename F>
    void fill(T* u, unsigned int size, F modifier) {
        for(unsigned int i=0; i<size;++i) {
            u[i]=modifier(rand<T>());
        }
    }

    template<typename T,unsigned int size>
    void fill(T u[size]) {
        for(unsigned int i=0; i<size;++i) {
            u[i]=rand<T>();
        }
    }
    template<typename T,unsigned int size, typename F>
    void fill(T u[size], F modifier) {
        for(unsigned int i=0; i<size;++i) {
            u[i]=modifier(rand<T>());
        }
    }

    template<typename T, typename storage>
    void fill(storage& u,unsigned int size) {
        for(unsigned int i=0; i<size;++i) {
            u[i]=rand<T>();
        }
    }
    template<typename T, typename storage, typename F>
    void fill(storage& u,unsigned int size, F modifier) {
        for(unsigned int i=0; i<size;++i) {
            u[i]=modifier(rand<T>());
        }
    }

    template<typename T, typename  storage>
    void fill(typename storage::iterator begin, typename storage::const_iterator end) {
        while(begin!=end) {
            (*begin)=(rand<T>());
            begin++;
        }
    }
    template<typename T, typename  storage, typename F>
    void fill(typename storage::iterator begin, typename storage::const_iterator end, F modifier) {
        while(begin!=end) {
            (*begin)=modifier(rand<T>());
            begin++;
        }
    }


    template<typename T>
    std::vector<T> randVector(unsigned int size) {
        std::vector<T> u(size);
        for(unsigned int i=0; i<size;++i) {
            u[i]=(rand<T>());
        }
        return u;
    }
    template<typename T, typename F>
    std::vector<T> randVector(unsigned int size, F modifier) {
        std::vector<T> u(size);
        for(unsigned int i=0; i<size;++i) {
            u[i]=modifier(rand<T>());
        }
        return u;
    }



    template<typename T, unsigned int size>
    std::vector<T> randVector() {
        std::vector<T> u(size);
        for(unsigned int i=0; i<size;++i) {
            u[i]=(rand<T>());
        }
        return u;
    }
    template<typename T, unsigned int size, typename F>
    std::vector<T> randVector(F modifier) {
        std::vector<T> u(size);
        for(unsigned int i=0; i<size;++i) {
            u[i]=modifier(rand<T>());
        }
        return u;
    }


    template<typename T, unsigned int size>
    std::array<T,size> randArray() {
        std::array<T,size> u;
        for(unsigned int i=0; i<size;++i) {
            u[i]=(rand<T>());
        }
        return u;
    }
    template<typename T, unsigned int size, typename F>
    std::array<T,size> randArray(F modifier) {
        std::array<T,size> u;
        for(unsigned int i=0; i<size;++i) {
            u[i]=modifier(rand<T>());
        }
        return u;
    }

    template<typename T,
        typename std::enable_if<(std::is_integral<T>::value && std::is_signed<T>::value),int>::type=0 >
    static auto rangeModifier(T start, T end) {
        return [=](auto&& a) {  
            return start+(a<0?a*-1:a)%(end-start);};
    }
    template<typename T,
        typename std::enable_if<(std::is_integral<T>::value && std::is_unsigned<T>::value),int>::type=0 >
    static auto rangeModifier(T start, T end) {
        return [=](auto&& a) {  
            return start+a%(end-start);};
    }

    template<typename T,
        typename std::enable_if<std::is_floating_point<T>::value,int>::type=0 >
    static auto rangeModifier(T start, T end) {
        return [=](auto&& a) {  return start+a*(end-start);};
    }

    template<typename T>
    auto randRange(T start, T end) {
        return rangeModifier(start,end)(rand<T>());
    }
};


/*
 * General Source description
 *
 * Perservative: A perservative source is a source for which getGenerator() always yields the same generator (same State) after some calls of newSource()
 *
 * Read more on SequenceSplitting
 */
template<typename Derived, typename GenImpl, bool perservative_>
struct RandomSourcePolicy {

    inline Derived newSource() {
        return getDerived().newSource();
    };

    inline GenImpl getGeneratorImpl() {
        return getDerived().getGeneratorImpl();
    };

    inline RandomGenerator<GenImpl> getGenerator() {
        return RandomGenerator<GenImpl>(getGeneratorImpl());
    };

    inline Derived& getDerived() {
        return static_cast<Derived&>(*this);
    };

    static const bool perservative = perservative_;
};

/*
 * RandomSpacing - RandomSeeding: Large period required
 *
 * SeedGenImpl: GeneratorImplementation to get the seed for initiating GenImpl.
 */
template<typename GenImpl=Xorshift1024star, bool perservative=true, typename SeedGenImpl = Splitmix64>
struct RandomSpacing: RandomSourcePolicy<
                                    RandomSpacing<GenImpl, perservative, SeedGenImpl>,
                                    GenImpl,
                                    perservative> {
    using Derived = RandomSpacing<GenImpl, perservative, SeedGenImpl>;
    SeedGenImpl seedgen;
    decltype(seedgen.next()) initState;


    RandomSpacing(Derived&& other): seedgen(std::move(other.seedgen)), initState(std::move(other.initState)) {};
    RandomSpacing(const Derived& other): seedgen(other.seedgen), initState(other.initState) {};

    template<typename Arg1, typename ...Args,
        typename std::enable_if<!std::is_same<typename std::decay<Arg1>::type, Derived>::value,int>::type=0 >
    RandomSpacing(Arg1&& arg1, Args&&... args): seedgen(RandomGenImplInitiator<SeedGenImpl>::get(std::forward<Arg1>(arg1),std::forward<Args>(args)...)), initState(seedgen.next()) {}
    RandomSpacing(): seedgen(RandomGenImplInitiator<SeedGenImpl>::get()), initState(seedgen.next()) {}

    Derived newSource() {
        if(!perservative) { initState=seedgen.next(); }
        return std::move(Derived(initState)); 
    }

    GenImpl getGeneratorImpl() {
        return RandomGenImplInitiator<GenImpl>::get(initState);
    }
};

/*
jump() jumps N steps forward.
Splitting should result in 2 streams that can split independently

             x
         x       x
       x   x   x   x 

Depth       Indices (each index represents 1 jump point, the tree structure describes the required separation)
0            1
1        2       3
2      4   5   6   7

Depth 0:
From (1) to (2) 2^0 = 0 jumps
From (1) to (3) 2^1 = 1 jumps

Depth 1:
From (2) to (4) 2^1   = 2 jumps
From (2) to (5) 2^1+1 = 3 jumps

From (3) to (6) 2^1+1 = 3 jumps
From (3) to (7) 2^1+2 = 4 jumps


Depth 2:
From (4) to (8)  2^2   = 4 jumps
From (4) to (9)  2^2+1 = 5 jumps

From (5) to (10) 2^2+1 = 5 jumps
From (5) to (11) 2^2+2 = 6 jumps

From (6) to (12) 2^2+2 = 6 jumps
From (6) to (13) 2^2+3 = 7 jumps

From (7) to (14) 2^2+3 = 7 jumps
From (7) to (15) 2^2+4 = 8 jumps

=> Jump from index i to left child j:  2^d+(i-2^d) = i jumps (yeah kinda useless)

New indices for left and right child of i:
Left:   i*2
Right:  i*2+1

One has to keep track of State i. getGenerator() is returning a Generator initialized with that State.
newSource() performs a splitting by returning a new Source for the right i*2+1 child. 
The current source then owns two sources i and the left child i*2 to be.
Opened implementation decision: Shall getGenerator() always return source i or i*2? (1 source is unused).
    - If getGenerator() always returns i, where it was created from.
      getGenerator() is constant in the sense that indermediate that the actual
      used generator can always be requested again and splitting does not effect
      the order number are generated. For this purpose, always two states and the actual index have to be stored per source.
      For sequence splitting, some states are always "wasted". I.e. split i
      creates i*2 and i*2+1. i*2 is bound to i while a generator has to be fixed on one sequence.
      Thus split from used sequence i consumes 2 additional sequences.
    - Another option is to abandon the old source and create two new sources.
      For this design the current generator can not be retrieved again by
      getGenerator() after a new source has been requested, as the current source is modified itself aswell.

The decision is between having independent source objects that can regenerate
independent sources more than once or only once, thus remaining in a wasted state.

A general approach, that does not conflict with other policies and gives more
freedom in usage of sources is the first approach.

Result:
The decision is between separating sources from generators, or combining both.
Both is possible - thus Policies are splitted in perservative and non-perservative.

Combining Generators with sources (this makes a Generator cabable of splitting etc.) requires perservative sources.
Strictly splitting generators from sources allows both and gives freedom to the overlaying usage and requirement for used algorithms.

Non-perservative generators can always decay to perservative generators by saving initial states and abandon one new created source
*/
template<typename GenImpl=Xorshift1024star, bool perservative=true, typename SeedGenImpl = Splitmix64>
struct SequenceSplitting: RandomSourcePolicy<
                                    SequenceSplitting<GenImpl>,
                                    GenImpl,
                                    perservative> {};

template<typename GenImpl, typename SeedGenImpl>
struct SequenceSplitting<GenImpl, true, SeedGenImpl>: RandomSourcePolicy<
                                    SequenceSplitting<GenImpl, true, SeedGenImpl>,
                                    GenImpl,
                                    true> {
    using Derived = SequenceSplitting<GenImpl, true, SeedGenImpl>;
    using StateType = typename GenImpl::StateType;
    static_assert(GenImpl::jumpAble,
                  "SequenceSplitting requires a generator supporting jump ahead.");
    StateType   defaultGenInitState;
    StateType   lastState;
    unsigned int index = 1;

    SequenceSplitting(Derived&& other): defaultGenInitState(std::move(other.defaultGenInitState)), lastState(std::move(other.lastState)), index(other.index) {};
    SequenceSplitting(const Derived& other): defaultGenInitState(other.defaultGenInitState), lastState(other.lastState), index(other.index) {};

    template<typename Arg1, typename ...Args,
        typename std::enable_if<!std::is_same<typename std::decay<Arg1>::type, Derived>::value,int>::type=0 >
    SequenceSplitting(Arg1&& arg1, Args&&... args): defaultGenInitState(RandomGenImplInitiator<GenImpl>::get(
                                              (RandomGenImplInitiator<SeedGenImpl>::get(std::forward<Arg1>(arg1),std::forward<Args>(args)...)).next()
                                            ).getState()), lastState(defaultGenInitState) {}
    SequenceSplitting(): defaultGenInitState(RandomGenImplInitiator<GenImpl>::get().getState()), lastState(defaultGenInitState) {}

    void initChild(StateType state, unsigned int index_=1) {
        defaultGenInitState = state;
        lastState = state;
        index = index_;
    }

    Derived newSource() {
        Derived source;
        GenImpl gen = RandomGenImplInitiator<GenImpl>::get(lastState);
        for(unsigned int i=0;i<index;++i) gen.jump();
        lastState = gen.getState();
        index*=2;
        gen.jump();
        source.initChild(gen.getState(),index+1);
        return std::move(source); 
    }
    GenImpl getGeneratorImpl() {
        return RandomGenImplInitiator<GenImpl>::get(defaultGenInitState);
    }
};

template<typename GenImpl, typename SeedGenImpl>
struct SequenceSplitting<GenImpl, false, SeedGenImpl>: RandomSourcePolicy<
                                    SequenceSplitting<GenImpl, false, SeedGenImpl>,
                                    GenImpl,
                                    false> {
    using Derived = SequenceSplitting<GenImpl, false, SeedGenImpl>;
    using StateType = typename GenImpl::StateType;
    static_assert(GenImpl::jumpAble,
                  "SequenceSplitting requires a generator supporting jump ahead.");
    StateType   state;
    unsigned int index = 1;

    SequenceSplitting(Derived&& other): state(std::move(other.state)), index(other.index) {};
    SequenceSplitting(const Derived& other): state(other.state), index(other.index) {};

    template<typename Arg1, typename ...Args,
        typename std::enable_if<!std::is_same<typename std::decay<Arg1>::type, Derived>::value,int>::type=0 >
    SequenceSplitting(Arg1&& arg1, Args&&... args): state(RandomGenImplInitiator<GenImpl>::get(
                                              (RandomGenImplInitiator<SeedGenImpl>::get(std::forward<Arg1>(arg1),std::forward<Args>(args)...)).next()
                                            ).getState()) {}
    SequenceSplitting(): state(RandomGenImplInitiator<GenImpl>::get().getState()) {}

    void initChild(StateType state, unsigned int index_=1) {
        state= state;
        index = index_;
    }

    Derived newSource() {
        Derived source;
        GenImpl gen = RandomGenImplInitiator<GenImpl>::get(state);
        for(unsigned int i=0;i<index;++i) gen.jump();
        state= gen.getState();
        index*=2;
        gen.jump();
        source.initChild(gen.getState(),index+1);
        return std::move(source); 
    }
    GenImpl getGeneratorImpl() {
        return RandomGenImplInitiator<GenImpl>::get(state);
    }
};

}

#endif // RandomGen_hpp_INCLUDED

