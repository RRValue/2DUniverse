#ifndef _BASE_NGRADEBEZIER_H_
#define _BASE_NGRADEBEZIER_H_

#include "HalfEdge2D/Base/Vector.h"

#include "HalfEdge2D/Base/StaticGroupElements.h"
#include "HalfEdge2D/Base/StaticPolynomialSolver.h"
#include "HalfEdge2D/Base/StaticBernsteinMatrix.h"

#include <map>
#include <vector>

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
    
    typedef std::pair<T, T> RangeLengthType;
    typedef std::map<T, T> RangeLengthMapType;
    typedef std::vector<T> LengthVectoeType;

public:
    typedef Eigen::Matrix<T, N, 1> ComponentValuesType;
    typedef Eigen::Matrix<T, 1, N> RowValuesType;
    typedef Eigen::Matrix<T, N, N> ParamTransformMatrixType;

public:
    typedef BezierParamType BezierPointsType;
    typedef Result Roots;

    static_assert(G <= 13, "13 Is max for StaticNGradeBezier");

public:
    StaticNGradeBezier() : 
        m_LengthCacheMax(size_t(std::pow(T(2), T(m_LengthCacheDepth)) + T(1))),
        m_LengthCacheStep(T(1) / (T(m_LengthCacheMax - 1)))
    {
        m_Params.setZero();
        m_DerivedParams[0].setZero();
        m_DerivedParams[1].setZero();
        m_DerivedParams[2].setZero();

        m_LengthDirty = true;
    }

    StaticNGradeBezier(const StaticNGradeBezier& other) :
        m_LengthCacheMax(size_t(std::pow(T(2), T(m_LengthCacheDepth)) + T(1))),
        m_LengthCacheStep(T(1) / (T(m_LengthCacheMax - 1)))
    {
        copy(other, *this);
    }

    StaticNGradeBezier(StaticNGradeBezier&& other) :
        m_LengthCacheMax(size_t(std::pow(T(2), T(m_LengthCacheDepth)) + T(1))),
        m_LengthCacheStep(T(1) / (T(m_LengthCacheMax - 1)))
    {
        move(std::move(other), *this);
    }

    StaticNGradeBezier& operator=(const StaticNGradeBezier& other)
    {
        copy(other, *this);

        return *this;
    }

    StaticNGradeBezier& operator=(StaticNGradeBezier&& other)
    {
        move(other, *this);

        return *this;
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
        if(!m_LengthDirty)
            return m_Length;

        // clear length related memeber
        m_Length = StaticIdentities.identityAdd<T>();
        m_RangeLengthMap.clear();
        m_LengthVector.clear();

        // update length
        m_LengthVector.push_back(T(0));

        lengthImpl(m_Params, m_LengthError.m_Epsilon, 0);

        m_LengthVector.push_back(m_Length);

        m_LengthDirty = false;

        // update length map
        if(m_LengthVector.size() == m_LengthCacheMax)
        {
            T a = T(0);

            for(const auto& l : m_LengthVector)
            {
                m_RangeLengthMap.insert(std::make_pair(a, l));
                a += m_LengthCacheStep;
            }
        }
        else
        {
            m_RangeLengthMap.insert(std::make_pair(T(0), T(0)));
            m_RangeLengthMap.insert(std::make_pair(T(0), m_Length));
        }

        m_LengthVector.clear();

        return m_Length;
    }

private:
    void copy(const StaticNGradeBezier& from, StaticNGradeBezier& to)
    {
        m_Params = from.m_Params;
        m_DerivedParams[0] = from.m_DerivedParams[0];
        m_DerivedParams[1] = from.m_DerivedParams[1];
        m_DerivedParams[2] = from.m_DerivedParams[2];
        m_RangeLengthMap = from.m_RangeLengthMap;
        m_LengthVector = from.m_LengthVector;
        m_Length = from.m_Length;
        m_LengthDirty = from.m_LengthDirty;

    }
    void swap(StaticNGradeBezier& from, StaticNGradeBezier& to)
    {
        std::swap(from.m_Params, to.m_Params);
        std::swap(from.m_DerivedParams[0], to.m_DerivedParams[0]);
        std::swap(from.m_DerivedParams[1], to.m_DerivedParams[1]);
        std::swap(from.m_DerivedParams[2], to.m_DerivedParams[2]);
        std::swap(from.m_RangeLengthMap, to.m_RangeLengthMap);
        std::swap(from.m_LengthVector, to.m_LengthVector);
        std::swap(from.m_Length, to.m_Length);
        std::swap(from.m_LengthDirty, to.m_LengthDirty);
    }

    void move(StaticNGradeBezier&& from, StaticNGradeBezier& to)
    {
        m_Params = std::move(from.m_Params);
        m_DerivedParams[0] = std::move(from.m_DerivedParams[0]);
        m_DerivedParams[1] = std::move(from.m_DerivedParams[1]);
        m_DerivedParams[2] = std::move(from.m_DerivedParams[2]);
        m_RangeLengthMap = std::move(from.m_RangeLengthMap);
        m_LengthVector = std::move(from.m_LengthVector);
        m_Length = std::move(from.m_Length);
        m_LengthDirty = std::move(from.m_LengthDirty);
    }

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

    template<unsigned int GRADE = G>
    void lengthImpl(const BezierParamType& points, const T& error, size_t depth)
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

            lengthImpl(l, error, depth + 1);

            if(depth < m_LengthCacheDepth)
                m_LengthVector.push_back(m_Length);

            lengthImpl(r, error, depth + 1);

            return;
        }

        m_Length += len_a;

        return;
    }

    template<>
    void lengthImpl<1>(const BezierParamType& points, const T& error, size_t depth)
    {
        m_Length = (points.col(1) - points.col(0)).norm();
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

    RangeLengthMapType m_RangeLengthMap;
    LengthVectoeType m_LengthVector;

    static const size_t m_LengthCacheDepth = 5; // will expand to 2^n + 1 values in m_RangeLengthMap 0, 1/2^n, 2/2^n ... 1
    const size_t m_LengthCacheMax;
    const T m_LengthCacheStep;
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
