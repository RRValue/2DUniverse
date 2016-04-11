#pragma once

#include "HalfEdge2D/Base/StaticBinomialRow.h"
#include "HalfEdge2D/Base/StaticGroupElements.h"

template <typename T, unsigned int N>
class StaticNGradeBlend : StaticBinomialRow<N - 1>
{
    static_assert(N <= 13, "13 Is max for StaticBlend<N>");

public:
    T blend(const std::array<T, N>& values, const float& alpha)
    {
        T result = StaticIdentities.identityAdd<T>();
        float a0 = (1.0f - alpha);
        float a1 = alpha;

        for(size_t i = 0; i < N; i++)
            result += values[i] * m_Factors[i] * std::powf(a0, (float)(N - i - 1)) * std::powf(a1, (float)i);

        return result;
    }
};

