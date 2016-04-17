#ifndef _BASE_STATICBERNSTEINMATRIX_H_
#define _BASE_STATICBERNSTEINMATRIX_H_

#include "HalfEdge2D/Base/StaticGroupElements.h"

template <typename T, unsigned int G, unsigned int N = G + 1>
struct StaticBernsteinMatrix
{
    typedef Eigen::Matrix<T, N, N> MatrixType;

    template<unsigned int M = N>
    inline const MatrixType& getBernsteinMatrix() const
    {
        static_assert(false, "not defned for this Grade");
    }

    template<>
    inline const MatrixType& getBernsteinMatrix<1>() const
    {
        static MatrixType expandMatrixD1 = []
        {
            MatrixType m;

            m(0, 0) = T(1);

            return m;
        }();

        return expandMatrixD1;
    }

    template<>
    inline const MatrixType& getBernsteinMatrix<2>() const
    {
        static MatrixType expandMatrixD2 = []
        {
            MatrixType m;

            m <<
                T(-1), T(1),
                T(1), T(0);

            return m;
        }();

        return expandMatrixD2;
    }

    template<>
    inline const MatrixType& getBernsteinMatrix<3>() const
    {
        static MatrixType expandMatrixD3 = []
        {
            MatrixType m;

            m <<
                T(1), T(-2), T(1),
                T(-2), T(2), T(0),
                T(1), T(0), T(0);

            return m;
        }();

        return expandMatrixD3;
    }

    template<>
    inline const MatrixType& getBernsteinMatrix<4>() const
    {
        static MatrixType expandMatrixD4 = []
        {
            MatrixType m;

            m <<
                T(-1), T(3), T(-3), T(1),
                T(3), T(-6), T(3), T(0),
                T(-3), T(3), T(0), T(0),
                T(1), T(0), T(0), T(0);

            return m;
        }();

        return expandMatrixD4;
    }
};

#endif //_BASE_STATICBERNSTEINMATRIX_H_
