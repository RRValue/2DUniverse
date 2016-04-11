#ifndef _BASE_STATICBINOMIALROW_H_
#define _BASE_STATICBINOMIALROW_H_

#include "HalfEdge2D/Base/StaticBinomialCoef.h"

#include <array>

template <unsigned int N>
class StaticBinomialRow
{
    static_assert(N <= 12, "12 Is max for StaticBinomialRow<N>");

protected:
    StaticBinomialRow() : m_Grade(N), m_Factors(std::array<float, N + 1>())
    {
        init<N>();
    }
    ~StaticBinomialRow()
    {

    }

    template<unsigned int K>
    void init()
    {
        init<K - 1>();

        m_Factors[K] = (float)StaticBinomialCoef<N, K>::m_Value;
    }

    template<>
    void init<0>()
    {
        m_Factors[0] = (float)StaticBinomialCoef<N, 0>::m_Value;
    }

protected:
    const unsigned int m_Grade;
    std::array<float, N + 1> m_Factors;
};

#endif //_BASE_STATICBINOMIALROW_H_
