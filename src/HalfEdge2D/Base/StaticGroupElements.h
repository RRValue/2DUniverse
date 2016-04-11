#ifndef _BASE_STATICGROUPELEMENT_H_
#define _BASE_STATICGROUPELEMENT_H_

#include "HalfEdge2D/Base/Vector.h"

struct Identites
{
    // identityAdd
    template<typename S>
    static S identityAdd()
    {
        static_assert(false, "undefined type for identityAdd");

        return S();
    }

    template<> static float identityAdd<float>() { return 0.0f; }
    template<> static double identityAdd<double>() { return 0.0; }
    template<> static Vec2f identityAdd<Vec2f>() { return Vec2f(0.0f, 0.0f); }

    // identityMult
    template<typename S>
    static S identityMult()
    {
        static_assert(false, "undefined type for identityAdd");

        return S();
    }

    template<> static float identityMult<float>() { return 1.0f; }
    template<> static double identityMult<double>() { return 1.0; }
    template<> static Vec2f identityMult<Vec2f>() { return Vec2f(1.0f, 1.0f); }
};

static Identites StaticIdentities;

#endif //_BASE_STATICGROUPELEMENT_H_
