#ifndef _BASE_NGRADEBEZIER_H_
#define _BASE_NGRADEBEZIER_H_

#include "HalfEdge2D/Base/Vector.h"

#include "HalfEdge2D/Base/StaticGroupElements.h"
#include "HalfEdge2D/Base/StaticPolynomialSolver.h"
#include "HalfEdge2D/Base/StaticBernsteinMatrix.h"

template <typename T, unsigned int G, unsigned int D, unsigned N = G + 1>
class StaticNGradeBezier : StaticPolynomialSolver<T, G>, public StaticBernsteinMatrix<T, G>
{
private:
    // typedefs
    typedef Eigen::Matrix<T, D, N> BezierParamType;
    typedef Eigen::Matrix<T, D, 1> BezierPointType;
    typedef Eigen::Matrix<T, D + 1, 1> TransformPointType;
    typedef Eigen::Matrix<T, D + 1, D + 1> TransformType;

public:
    typedef Eigen::Matrix<T, N, 1> ComponentValuesType;
    typedef Eigen::Matrix<T, 1, N> RowValuesType;
    typedef Eigen::Matrix<T, N, N> ParamTransformMatrixType;

public:
    typedef BezierParamType BezierPointsType;
    typedef Result Roots;

    static_assert(G <= 13, "13 Is max for StaticNGradeBezier");

public:
    StaticNGradeBezier()
    {
        m_Params.setZero();
        m_DerivedParams[0].setZero();
        m_DerivedParams[1].setZero();
        m_DerivedParams[2].setZero();
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
        return derived<0>(alpha);
    }

    BezierPointType tangentAt(const T& alpha) const
    {
        return derived<1>(alpha).normalized();
    }

    template<unsigned int Dim = D>
    BezierPointType normalAt(const T& alpha) const
    {
        static_assert(false, "normalAt not defined for this Dimension")
    }

    template<>
    BezierPointType normalAt<2>(const T& alpha) const
    {
        BezierPointType der1 = tangentAt(alpha);

        return BezierPointType(-der1(1), der1(0));
    }

    template<>
    BezierPointType normalAt<3>(const T& alpha) const
    {
        BezierPointType der1 = derived<1>(alpha);
        BezierPointType der2 = derived<2>(alpha);

        return der1.cross(der1.cross(der2)).normalized();
    }

    template<unsigned int Dim = D>
    BezierPointType biNormalAt(const T& alpha) const
    {
        static_assert(false, "biNormalAt not defined for this Dimension")
    }

    template<>
    BezierPointType biNormalAt<3>(const T& alpha) const
    {
        BezierPointType der1 = derived<1>(alpha);
        BezierPointType der2 = derived<1>(alpha);

        return der1.cross(der2).normalized();
    }

    template<unsigned int Dim = D>
    T curvationAt(const T& alpha) const
    {
        static_assert(false, "biNormalAt not defined for this Dimension")
    }

    template<>
    T curvationAt<2>(const T& alpha) const
    {
        BezierPointType der1 = derived<1>(alpha);
        BezierPointType der2 = derived<2>(alpha);

        T a = (der1(0) * der2(1)) - (der1(1) * der2(0));
        T b = (der1(0) * der1(0)) + (der2(0) * der2(0));

        b = std::sqrt(std::pow(b, T(3)));

        return a / b;
    }

    template<>
    T curvationAt<3>(const T& alpha) const
    {
        BezierPointType der1 = derived<1>(alpha);
        BezierPointType der2 = derived<2>(alpha);

        return der1.cross(der2).norm() / std::pow(der1.norm(), T(3));
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

    Roots componentRoots(const size_t& c) const
    {
        return solve(m_DerivedParams[0].row(c));
    }

private:
    void updateParams()
    {
        m_DerivedParams[0] = m_Params * getDerivedBernsteinMatrix<0>();
        m_DerivedParams[1] = m_Params * getDerivedBernsteinMatrix<1>();
        m_DerivedParams[2] = m_Params * getDerivedBernsteinMatrix<2>();
    }

    template<unsigned int DEV>
    inline BezierPointType derived(const float& alpha) const
    {
        ComponentValuesType a_vec;

        T ca = StaticIdentities.identityMult<T>();

        for(size_t i = 0; i < N - DEV; i++)
        {
            a_vec[N - 1 - i - DEV] = ca;

            ca *= alpha;
        }

        return m_DerivedParams[DEV] * a_vec;
    }

private:
    static const size_t m_Grade = G;
    static const size_t m_Dim = D;
    static const size_t m_NumPoints = N;
    static const size_t m_Derivations = 2;

    BezierParamType m_Params;
    BezierParamType m_DerivedParams[m_Derivations + 1];
};

typedef StaticNGradeBezier<float, 1, 2> Line2F;
typedef StaticNGradeBezier<float, 2, 2> QBezier2F;
typedef StaticNGradeBezier<float, 3, 2> CBezier2F;

typedef StaticNGradeBezier<float, 1, 3> Line3F;
typedef StaticNGradeBezier<float, 2, 3> QBezier3F;
typedef StaticNGradeBezier<float, 3, 3> CBezier3F;

typedef StaticNGradeBezier<double, 1, 2> Line2D;
typedef StaticNGradeBezier<double, 2, 2> QBezier2D;
typedef StaticNGradeBezier<double, 3, 2> CBezier2D;

typedef StaticNGradeBezier<double, 1, 3> Line3D;
typedef StaticNGradeBezier<double, 2, 3> QBezier3D;
typedef StaticNGradeBezier<double, 3, 3> CBezier3D;

#endif //_BASE_NGRADEBEZIER_H_
