#ifndef _BASE_STATICBINOMIALCOEF_H_
#define _BASE_STATICBINOMIALCOEF_H_

#include "HalfEdge2D/Base/StaticFactorial.h"

template <unsigned int N, unsigned int K>
struct StaticBinomialCoef
{
    static_assert(N <= 12, "12 Is max for StaticBinomial");
    static_assert(K <= N, "K not smaller equal to K");

    static const unsigned int m_Value = StaticFactorial<N>::m_Value / (StaticFactorial<K>::m_Value * StaticFactorial<N - K>::m_Value);
};

#endif //_BASE_STATICBINOMIALCOEF_H_
