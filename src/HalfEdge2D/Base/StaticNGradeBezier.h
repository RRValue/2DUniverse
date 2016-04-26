#ifndef _BASE_NGRADEBEZIER_H_
#define _BASE_NGRADEBEZIER_H_

#include "HalfEdge2D/Base/Vector.h"

#include "HalfEdge2D/Base/StaticGroupElements.h"
#include "HalfEdge2D/Base/StaticPolynomialSolver.h"
#include "HalfEdge2D/Base/StaticBernsteinMatrix.h"

template<typename T>
struct StaticNGradeBezierLengthErrorTolerance
{
    static T m_Epsilon;
};

template<> float StaticNGradeBezierLengthErrorTolerance<float>::m_Epsilon = 1e-05f;
template<> double StaticNGradeBezierLengthErrorTolerance<double>::m_Epsilon = 1e-05;

template <typename T, unsigned int G, unsigned int D, unsigned N = G + 1>
class StaticNGradeBezier : StaticPolynomialSolver<T, G>, public StaticBernsteinMatrix<T, G>
{
private:
    StaticNGradeBezierLengthErrorTolerance<T> m_LengthError;

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

        m_LengthDirty = true;
    }

    StaticNGradeBezier(const StaticNGradeBezier& other)
    {
        m_Params = other.m_Params;
        m_DerivedParams[0] = other.m_DerivedParams[0];
        m_DerivedParams[1] = other.m_DerivedParams[1];
        m_DerivedParams[2] = other.m_DerivedParams[2];
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

        m_LengthDirty = true;
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

        return der1.cross(der2).cross(der1).normalized();
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

    void splitAt(const float& a, StaticNGradeBezier& l, StaticNGradeBezier& r)
    {
        splitAtImpl(m_Params, a, l.m_Params, r.m_Params);
        
        l.updateParams();
        r.updateParams();

        l.m_LengthDirty = true;
        r.m_LengthDirty = true;
    }

    T getLength()
    {
        if(m_LengthDirty)
        {
            m_Length = StaticIdentities.identityAdd<T>();

            lengthImpl(m_Params, m_LengthError.m_Epsilon);

            m_LengthDirty = false;
        }

        return m_Length;
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

    void splitAtImpl(const BezierParamType& p, const float& a, BezierParamType& l, BezierParamType& r)
    {
        // sources:
        /// http://www.realtimerendering.com/resources/GraphicsGems/gems.html#gems
        /// http://www.realtimerendering.com/resources/GraphicsGems/gems/NearestPoint.c

        BezierPointType temp[N][N];

        // Copy control points
        for(unsigned int j = 0; j < N; j++)
            temp[0][j] = p.col(j);

        // Triangle computation
        T a0 = a;
        T a1 = StaticIdentities.identityMult<T>() - a;

        for(unsigned int i = 1; i < N; i++)
            for(unsigned int j = 0; j < N - i; j++)
                temp[i][j] = (a1 * temp[i - 1][j]) + (a0 * temp[i - 1][j + 1]);

        for(unsigned int j = 0; j < N; j++)
            l.col(j) = temp[j][0];

        for(unsigned int j = 0; j < N; j++)
            r.col(j) = temp[N - j - 1][j];
    }

    void lengthImpl(const BezierParamType& points, const T& error)
    {
        BezierParamType l, r;

        T len_a = StaticIdentities.identityAdd<T>();
        T len_c = StaticIdentities.identityAdd<T>();
        T s = StaticIdentities.identityMult<T>() / T(2);

        for(unsigned int i = 0; i < N - 1; i++)
            len_a += (points.col(i + 1) - points.col(i)).norm();

        len_c = (points.col(N - 1) - points.col(0)).norm();

        if(std::abs(len_a - len_c) > error)
        {
            splitAtImpl(points, s, l, r);
            lengthImpl(l, error);
            lengthImpl(r, error);

            return;
        }

        m_Length += len_a;

        return;
    }

private:
    static const size_t m_Grade = G;
    static const size_t m_Dim = D;
    static const size_t m_NumPoints = N;
    static const size_t m_Derivations = 2;

    BezierParamType m_Params;
    BezierParamType m_DerivedParams[m_Derivations + 1];

    bool m_LengthDirty;
    T m_Length;
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
