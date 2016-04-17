#ifndef _BASE_NGRADEBEZIER_H_
#define _BASE_NGRADEBEZIER_H_

#include "HalfEdge2D/Base/Vector.h"

#include "HalfEdge2D/Base/StaticGroupElements.h"
#include "HalfEdge2D/Base/StaticPolynomialSolver.h"
#include "HalfEdge2D/Base/StaticBernsteinMatrix.h"

#include <cassert>

template <typename T, unsigned int G, unsigned int D, unsigned N = G + 1>
class StaticNGradeBezier : StaticPolynomialSolver<T, G>, StaticBernsteinMatrix<T, G>
{
private:
    // typedefs
    typedef Eigen::Matrix<T, D, N> BezierParamType;
    typedef Eigen::Matrix<T, D, 1> BezierPointType;
    typedef Eigen::Matrix<T, N, 1> ComponentValuesType;
    typedef Eigen::Matrix<T, D + 1, 1> TransformPointType;
    typedef Eigen::Matrix<T, D + 1, D + 1> TransformType;
    typedef Eigen::Matrix<T, N, N> ExpandMatrixType;
    typedef std::vector<T> RootsVecType;

public:
    typedef BezierParamType BezierPointsType;

    static_assert(G <= 13, "13 Is max for StaticNGradeBezier");

public:
    StaticNGradeBezier()
    {
        m_Params.setZero();
        m_ExpandParams.setZero();
    }

    BezierPointType getPoint(const size_t& idx) const
    {
        return m_Params.col(idx);
    }

    const BezierPointsType& getPoints() const
    {
        return m_Params;
    }

    void setPoint(const size_t& idx, const BezierPointType& p)
    {
        m_Params.col(idx) = p;

        updateParams();
    }

    BezierPointType pointAt(const T& alpha) const
    {
        ComponentValuesType a_vec;

        float ca = StaticIdentities.identityMult<T>();

        for(size_t i = 0; i < N; i++)
        {
            a_vec[N - 1 - i] = ca;

            ca *= alpha;
        }

        return m_ExpandParams * a_vec;
    }

    void transform(const TransformType& m)
    {
        TransformPointType p;

        for(size_t i = 0; i < N; i++)
        {
            for(size_t j = 0; j < D; j++)
                p(j) = m_Params(j, i);

            p(D) = StaticIdentities.identityMult<T>();

            p = m * p;

            for(size_t j = 0; j < D; j++)
                m_Params(j, i) = p(j);
        }

        updateParams();
    }

    RootsVecType componentRoots(const size_t& c) const
    {
        Result res = solve(m_ExpandParams.row(c));

        RootsVecType result;

        for(size_t i = 0; i < res.m_Solutions; i++)
            result.push_back(res[i]);

        return result;
    }

private:
    void updateParams()
    {
        m_ExpandParams = m_Params * getBernsteinMatrix();
    }

private:
    BezierParamType m_Params;
    BezierParamType m_ExpandParams;
    
    static const size_t m_Grade = G;
    static const size_t m_Dim = D;
    static const size_t m_NumPoints = N;
};

#endif //_BASE_NGRADEBEZIER_H_
