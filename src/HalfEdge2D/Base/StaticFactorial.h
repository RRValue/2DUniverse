#ifndef _BASE_STATICFACTORIAL_H_
#define _BASE_STATICFACTORIAL_H_

template <unsigned int N>
struct StaticFactorial
{
    static_assert(N < 13, "12 Is max for StaticFac");

    static const unsigned int m_Value = N * StaticFactorial<N - 1>::m_Value;
};

template <>
struct StaticFactorial<0>
{
    static const unsigned int m_Value = 1;
};

#endif //_BASE_STATICFACTORIAL_H_
