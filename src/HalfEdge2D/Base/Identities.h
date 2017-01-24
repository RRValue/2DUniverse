#ifndef _BASE_IDENTITIES_H_
#define _BASE_IDENTITIES_H_

#include "HalfEdge2D/Base/Vector.h"

struct IdentitiesTpl
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
    template<> static Vec2d identityAdd<Vec2d>() { return Vec2d(0.0, 0.0); }

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
    template<> static Vec2d identityMult<Vec2d>() { return Vec2d(1.0, 1.0); }
};

static IdentitiesTpl Identities;

#endif //_BASE_IDENTITIES_H_
