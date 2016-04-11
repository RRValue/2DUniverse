#ifndef _BASE_STATICGROUPELEMENT_H_
#define _BASE_STATICGROUPELEMENT_H_

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

    // identityMult
    template<typename S>
    static S identityMult()
    {
        static_assert(false, "undefined type for identityAdd");

        return S();
    }

    template<> static float identityMult<float>() { return 1.0f; }
    template<> static double identityMult<double>() { return 1.0; }
};

static Identites StaticIdentities;

#endif //_BASE_STATICGROUPELEMENT_H_
