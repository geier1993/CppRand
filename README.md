Splitmix64 and Xorshift1024* with RandomSpacing and SequenceSpitting policy for parallel computing.

Splitmix64 is used to seed Xorshift1024* (as xorshift1024* needs to be initiated
with a non-zero 1024 bit state.). More generator implementations and policies
are possible.

   * <http://xoroshiro.di.unimi.it/#intro>

# Running Tests

```` {.bash}
mkdir build
meson build
cd build
ninja test
````


# Usage

see `test/*` for possible usage.

```` {.cpp}
#include "RandomGenerator.hpp"
using namespace PRNG;

// Initiating a source
// In these examples, a source for Xorshift1024* is created in perservative
// mode. Generators are initiated by using a Splitmix64 generator.
RandomSpacing<Xorshift1024star,true,Splitmix64>    source;       // Random seed
// Default template parameters, same as above
RandomSpacing<>                                    source;       // Random seed

RandomGenerator<Xorshift1024star>   gen = source.getGenerator();

gen.rand<int>(); 
gen.randFloat(); 
gen.randDouble(); 
gen.randInt(); 
gen.randInt8(); 
gen.randInt16(); 
gen.randInt32(); 
gen.randInt64(); 
gen.randUInt8(); 
gen.randUInt16(); 
gen.randUInt32(); 
gen.randUInt64(); 

gen.randRange(0,12);
gen.randRange(-1.0,2.0);


int a1[10];
gen.fill(a1,10);

int* a2 = new int[10];
gen.fill(a2,10);

std::vector<int> v1(10);
gen.fill<int>(v1,10);

std::vector<int> v2(10);
gen.fill<int,std::vector<int> >(v2.begin(),v2.end());

std::vector<int> v3 = gen().randVector<int>(10);

std::vector<int> v4 = gen().randVector<int,10>();

std::array<int,10> a3 = gen().randArray<int,10>();


// Sources
RandomSpacing<Xorshift1024star,true,Splitmix64>    source(seed);
SequenceSpitting<Xorshift1024star,true,Splitmix64> source;       // Random seed
SequenceSpitting<Xorshift1024star,true,Splitmix64> source(seed);

// Difference betwees perservative/non-perservative generators
SequenceSpitting<Xorshift1024star,true,Splitmix64> source1(seed);
gen1 = source1.getGenerator();
source2 = source1.newSource();
gen2 = source1.getGenerator(); // Same as gen1
gen3 = source2.getGenerator(); // Some different generator

SequenceSpitting<Xorshift1024star,false,Splitmix64> source3(seed);
gen4 = source3.getGenerator();
source4 = source3.newSource();
gen5 = source3.getGenerator(); // different from gen4
gen6 = source4.getGenerator(); // different from gen4 and gen5 
````

# TODO

Write tests using Catch
