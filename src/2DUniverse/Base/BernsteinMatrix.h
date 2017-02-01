#ifndef _BASE_BERNSTEINMATRIX_H_
#define _BASE_BERNSTEINMATRIX_H_

#include "2DUniverse/Base/Identities.h"

#include <array>
#include <algorithm>

template<typename T>
struct BernsteinMatrixTypeTraits
    {
    BernsteinMatrixTypeTraits()
        {
        static_assert(false, "BernsteinMatrix not defined for this Type");
    }
};

template<> struct BernsteinMatrixTypeTraits<float> {};
template<> struct BernsteinMatrixTypeTraits<double> {};
template<> struct BernsteinMatrixTypeTraits<int> {};
template<> struct BernsteinMatrixTypeTraits<char> {};
template<> struct BernsteinMatrixTypeTraits<long long> {};

template <typename T, size_t G>
class BernsteinMatrix
            {
public:
    typedef typename Eigen::Matrix<T, G + 1, G + 1> MatrixType;
    typedef typename Eigen::Matrix<T, G + 1, 1> MatrixRowType;
    typedef typename std::array<MatrixType, G + 1> MatrixArrayType;

public:
    inline static const MatrixType& getBernsteinMatrix(const size_t& idx)
    {
        if(!m_Initialized)
            initBersteinMatrices();

        return m_BernsteinMatrices[idx];
    }

private:
    inline static void initBersteinMatrices()
    {
        for(size_t i = 0; i <= G; i++)
            setDerivedBernsteinMatrix(i);

        m_Initialized = true;
    }

    inline static T sign(const size_t& v)
    {
        return v % 2 == 0 ? T(1) : T(-1);
    }

    static unsigned long long fakulty(const unsigned long long& v)
    {
        return (v != 0) ? (v * fakulty(v - 1)) : (1);
    }

    static T binom(const unsigned long long& n, const unsigned long long& k)
    {
        return (k > n) ? (0) : (T(fakulty(n) / (fakulty(k) * (fakulty(n - k)))));
    }

    inline static void setDerivedBernsteinMatrix(const size_t& idx)
    {
            MatrixType m;

        m.setZero();

        for(size_t i = 0; i <= G - idx; i++)
            for(size_t j = 0; j <= G - idx; j++)
                m(i, j) =
                    binom((unsigned long long)G - idx, (unsigned long long)j) *
                    binom((unsigned long long)(G - idx - j), (unsigned long long)i) *
                    sign(j + i + G + idx);

        m_BernsteinMatrices[idx] = m;
    }

private:
    static bool m_Initialized;
    static MatrixArrayType m_BernsteinMatrices;

private:
    static const BernsteinMatrixTypeTraits<T> m_Traits;
};

template <typename T, size_t G>
bool BernsteinMatrix<T, G>::m_Initialized = false;

template <typename T, size_t G>
std::array<Eigen::Matrix<T, G + 1, G + 1>, G + 1> BernsteinMatrix<T, G>::m_BernsteinMatrices = std::array<Eigen::Matrix<T, G + 1, G + 1>, G + 1>();

#endif //_BASE_BERNSTEINMATRIX_H_
