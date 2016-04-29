#ifndef _BASE_BERNSTEINMATRIX_H_
#define _BASE_BERNSTEINMATRIX_H_

#include "HalfEdge2D/Base/GroupElements.h"

template <typename T, unsigned int G, unsigned int N = G + 1>
struct BernsteinMatrix
{
    typedef Eigen::Matrix<T, N, N> MatrixType;
    typedef Eigen::Matrix<T, N, 1> MatrixRowType;

    template<unsigned int DEV>
    inline static MatrixType& getDerivedBernsteinMatrix()
    {
        static MatrixType mat = []
        {
            MatrixType d_mat = getDerivedBernsteinMatrix<DEV - 1>();

            MatrixRowType dvalues;
            dvalues.setZero();

            float cv = float(N - DEV);
            for(size_t i = 0; i < N - DEV + 1; i++)
            {
                dvalues[i] = cv;
                cv -= T(1);
            }

            for(size_t i = 0; i < N; i++)
                d_mat.col(i) *= dvalues(i);

            return d_mat;
        }();

        return mat;
    }

    template<>
    inline static MatrixType& getDerivedBernsteinMatrix<0>()
    {
        return getBernsteinMatrix();
    }

private:
    template<unsigned int M = N>
    inline static MatrixType& getBernsteinMatrix()
    {
        static_assert(false, "not defned for this Grade");
    }

    template<>
    inline static MatrixType& getBernsteinMatrix<1>()
    {
        static MatrixType mat = []
        {
            MatrixType m;

            m(0, 0) = T(1);

            return m;
        }();

        return mat;
    }

    template<>
    inline static MatrixType& getBernsteinMatrix<2>()
    {
        static MatrixType mat = []
        {
            MatrixType m;

            m <<
                T(-1), T(1),
                T(1), T(0);

            return m;
        }();

        return mat;
    }

    template<>
    inline static MatrixType& getBernsteinMatrix<3>()
    {
        static MatrixType mat = []
        {
            MatrixType m;

            m <<
                T(1), T(-2), T(1),
                T(-2), T(2), T(0),
                T(1), T(0), T(0);

            return m;
        }();

        return mat;
    }

    template<>
    inline static MatrixType& getBernsteinMatrix<4>()
    {
        static MatrixType mat = []
        {
            MatrixType m;

            m <<
                T(-1), T(3), T(-3), T(1),
                T(3), T(-6), T(3), T(0),
                T(-3), T(3), T(0), T(0),
                T(1), T(0), T(0), T(0);

            return m;
        }();

        return mat;
    }
};

#endif //_BASE_BERNSTEINMATRIX_H_
