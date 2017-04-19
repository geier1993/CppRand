#include "RandomGenerators.hpp" 

#include <iostream>
#include <string>
#include <assert.h>

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

auto simpleTestPersSource = [](auto source, bool verb) {
    auto gen1 = source.getGenerator();
    auto vec1 = gen1.template randVector<int>(10);

    auto source2 = source.newSource();

    auto gen2 = source.getGenerator();
    auto vec2 = gen2.template randVector<int>(10);
    auto gen3 = source2.getGenerator();
    auto vec3 = gen3.template randVector<int>(10);

    if(verb) {
        std::cout << "Vector1:\t" << vecToString(vec1) << std::endl;
        std::cout << "Vector2:\t" << vecToString(vec2) << std::endl;
        std::cout << "Vector3:\t" << vecToString(vec3) << std::endl;
    }
    assert(vecEqual(vec1,vec2));
    assert(!vecEqual(vec1,vec3));
};

auto simpleTestNonPersSource = [](auto&& source, bool verb) {
    auto gen1 = source.getGenerator();
    auto vec1 = gen1.template randVector<int>(10);

    auto source2 = source.newSource();

    auto gen2 = source.getGenerator();
    auto vec2 = gen2.template randVector<int>(10);
    auto gen3 = source2.getGenerator();
    auto vec3 = gen3.template randVector<int>(10);

    if(verb) {
        std::cout << "Vector1:\t" << vecToString(vec1) << std::endl;
        std::cout << "Vector2:\t" << vecToString(vec2) << std::endl;
        std::cout << "Vector3:\t" << vecToString(vec3) << std::endl;
    }
    assert(!vecEqual(vec1,vec2));
    assert(!vecEqual(vec1,vec3));
    assert(!vecEqual(vec2,vec3));
};

int main() {
    int seed=123;
    std::cout << "=== Test perservative source Random Spacing===" << std::endl;
    RandomSpacing<Xorshift1024star,true,Splitmix64> rsPersSource(seed);
    simpleTestPersSource(rsPersSource,true);

    std::cout << std::endl;
    std::cout << "=== Test perservative source Sequence Splitting===" << std::endl;
    SequenceSplitting<Xorshift1024star,true,Splitmix64> ssPersSource(seed);
    simpleTestPersSource(ssPersSource,true);

    std::cout << std::endl;
    std::cout << "=== Test non perservative source Random Spacing===" << std::endl;
    RandomSpacing<Xorshift1024star,false,Splitmix64> rsNonPersSource(seed);
    simpleTestNonPersSource(rsNonPersSource,true);

    std::cout << std::endl;
    std::cout << "=== Test non perservative source Sequence Splitting===" << std::endl;
    SequenceSplitting<Xorshift1024star,false,Splitmix64> ssNonPersSource(seed);
    simpleTestNonPersSource(ssNonPersSource,true);



    return(0);
}
