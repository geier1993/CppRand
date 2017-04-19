#ifndef GeneratorImplementation_hpp_INCLUDED
#define GeneratorImplementation_hpp_INCLUDED

namespace PRNG {

    template<typename Derived, bool jumpAble>
    struct JumpableGeneratorImplementation {};

    template<typename Derived>
    struct JumpableGeneratorImplementation<Derived,true> {
        inline void jump() {
            getDerived().jump();
        }
        inline Derived& getDerived() {
            return static_cast<Derived&>(*this);
        }
        inline Derived& getConstDerived() const {
            return static_cast<Derived&>(*this);
        }

        static const bool jumpAble= true;
    };
    template<typename Derived>
    struct JumpableGeneratorImplementation<Derived,false> {
        static const bool jumpAble= false;
    };


    template<typename Derived, bool jumpAble_>
    struct GeneratorImplementation: JumpableGeneratorImplementation<Derived,jumpAble_> {
        inline auto next() {
            return getDerived().next();
        }

        inline auto getState() const {
            return getConstDerived().getState();
        }

        inline Derived& getDerived() {
            return static_cast<Derived&>(*this);
        }
        inline Derived& getConstDerived() const {
            return static_cast<Derived&>(*this);
        }

    };

}

#endif // GeneratorImplementation_hpp_INCLUDED

