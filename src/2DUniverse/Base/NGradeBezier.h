#ifndef _BASE_NGRADEBEZIER_H_
#define _BASE_NGRADEBEZIER_H_

#include "2DUniverse/Base/Identities.h"
#include "2DUniverse/Base/PolynomialSolver.h"
#include "2DUniverse/Base/BezierParameter.h"

#include <map>
#include <vector>
#include <array>
#include <algorithm>

template<typename T>
struct NGradeBezierLengthErrorTolerance
{
    static T m_Epsilon;
};

template<> float NGradeBezierLengthErrorTolerance<float>::m_Epsilon = 1e-05f;
template<> double NGradeBezierLengthErrorTolerance<double>::m_Epsilon = 1e-05;

template <typename T, unsigned int G, unsigned int D>
class NGradeBezier : PolynomialSolver<T, G>, public BezierParameter<T, G, D>
{
private:
    NGradeBezierLengthErrorTolerance<T> m_LengthError;

private:
    typedef Eigen::Matrix<T, D, G + 1> BezierParamType;
    typedef Eigen::Matrix<T, D, 1> BezierPointType;
    typedef Eigen::Matrix<T, D + 1, 1> TransformPointType;
    typedef Eigen::Matrix<T, D + 1, D + 1> TransformType;

    typedef typename std::pair<T, T> RangeLengthType;
    typedef typename std::map<T, T> RangeLengthMapType;
    typedef typename RangeLengthMapType::const_iterator RangeLengthMapContIterType;
    typedef typename std::vector<T> LengthVectoeType;

public:
    typedef BezierParamType BezierPointsType;
    typedef Result Roots;

    static_assert(G <= 13, "13 Is max for NGradeBezier");

public:
    NGradeBezier() :
        m_LengthCacheMax(size_t(std::pow(T(2), T(m_LengthCacheDepth)) + T(1))),
        m_LengthCacheStep(T(1) / (T(m_LengthCacheMax - 1)))
    {
        m_LengthDirty = true;
    }

    NGradeBezier(const NGradeBezier& other) :
        m_LengthCacheMax(size_t(std::pow(T(2), T(m_LengthCacheDepth)) + T(1))),
        m_LengthCacheStep(T(1) / (T(m_LengthCacheMax - 1)))
    {
        copy(other, *this);
    }

    NGradeBezier(NGradeBezier&& other) :
        m_LengthCacheMax(size_t(std::pow(T(2), T(m_LengthCacheDepth)) + T(1))),
        m_LengthCacheStep(T(1) / (T(m_LengthCacheMax - 1)))
    {
        move(std::move(other), *this);
    }

    NGradeBezier& operator=(const NGradeBezier& other)
    {
        copy(other, *this);

        return *this;
    }

    NGradeBezier& operator=(NGradeBezier&& other)
    {
        move(std::move(other), *this);

        return *this;
    }

    BezierPointType getPoint(const size_t& idx) const
    {
        return get(idx);
    }

    const BezierPointsType& getPoints() const
    {
        return m_Parameter;
    }

    void setPoint(const size_t& idx, const BezierPointType& p)
    {
        set(idx, p);

        m_LengthDirty = true;
    }

    BezierPointType pointAt(const T& alpha) const
    {
        return get(0, alpha);
    }

    BezierPointType pointAtL(const T& alpha)
    {
        if(m_LengthDirty)
            updateLength();

        return get(0, getAlphaAtLength(alpha * m_Length));
    }

    BezierPointType tangentAt(const T& alpha) const
    {
        return get(1, alpha).normalized();
    }

    BezierPointType tangentAtL(const T& alpha) const
    {
        if(m_LengthDirty)
            updateLength();

        return tangentAt(getAlphaAtLength(alpha * m_Length));
    }

    BezierPointType normalAt(const T& alpha) const
    {
        return frenetAt(1, alpha);
    }

    BezierPointType normalAtL(const T& alpha)
    {
        if(m_LengthDirty)
            updateLength();

        return normalAt(getAlphaAtLength(alpha * m_Length));
    }

    BezierPointType biNormalAt(const T& alpha) const
    {
        return frenetAt(2, alpha);
    }

    BezierPointType biNormalAtL(const T& alpha)
    {
        if(m_LengthDirty)
            updateLength();

        return biNormalAt(getAlphaAtLength(alpha * m_Length));
    }

    BezierPointType frenetAt(const size_t& d, const T& alpha) const
    {
        if(d == 0)
            return tangentAt(alpha);

        BezierPointType der = get(d + 1, alpha);
        BezierPointType frenet;

        for(size_t i = 1; i <= d; i++)
        {
            frenet = frenetAt(i - 1, alpha);
            der -= der.dot(frenet) * frenet;
        }

        return der.normalized();
    }

    BezierPointType frenetAtL(const size_t& d, const T& alpha)
    {
        if(m_LengthDirty)
            updateLength();

        return frenetAt(d, getAlphaAtLength(alpha * m_Length));
    }

    template<unsigned int Dim = D>
    T curvationAt(const T& alpha) const
    {
        static_assert(false, "biNormalAt not defined for this Dimension")
    }

    template<>
    T curvationAt<2>(const T& alpha) const
    {
        BezierPointType der1 = get(1, alpha);
        BezierPointType der2 = get(2, alpha);

        T a = (der1(0) * der2(1)) - (der1(1) * der2(0));
        T b = (der1(0) * der1(0)) + (der2(0) * der2(0));

        b = std::sqrt(std::pow(b, T(3)));

        return a / b;
    }

    template<>
    T curvationAt<3>(const T& alpha) const
    {
        BezierPointType der1 = get(1, alpha);
        BezierPointType der2 = get(2, alpha);

        return der1.cross(der2).norm() / std::pow(der1.norm(), T(3));
    }

    T curvationAtL(const T& alpha)
    {
        if(m_LengthDirty)
            updateLength();

        return curvationAt<D>(getAlphaAtLength(alpha * m_Length));
    }

    void transform(const TransformType& m)
    {
        TransformPointType p;

        for(size_t i = 0; i <= G; i++)
        {
            for(size_t j = 0; j < D; j++)
                p(j) = m_Parameter(j, i);

            p(D) = Identities.identityMult<T>();

            p = m * p;

            for(size_t j = 0; j < D; j++)
                m_Parameter(j, i) = p(j);
        }

        updateParameter();

        m_LengthDirty = true;
    }

    Roots componentRoots(const size_t& c) const
    {
        return solve(m_DerivedParameter[0].row(c));
    }

    void splitAt(const float& a, NGradeBezier& l, NGradeBezier& r)
    {
        splitAtImpl(m_Parameter, a, l.m_Parameter, r.m_Parameter);

        l.updateParameter();
        r.updateParameter();

        l.m_LengthDirty = true;
        r.m_LengthDirty = true;
    }

    T getLength()
    {
        if(m_LengthDirty)
            updateLength();

        return m_Length;
    }

    T getLength(const T& a)
    {
        if(m_LengthDirty)
            updateLength();

        auto i = --m_RangeLengthMap.upper_bound(a);

        if(i->first == a)
            return i->second;

        return getLengthImpl(*i, a);
    }

    T getLength(const T& from, const T& to)
    {
        if(m_LengthDirty)
            updateLength();

        auto i0 = --m_RangeLengthMap.upper_bound(from);
        auto i1 = --m_RangeLengthMap.upper_bound(to);

        if(i0->first == from && i1->first == to)
            return i1->second - i0->second;
        else if(i0->first == from)
            return (++i1)->second - getLengthImpl(*i0, to);
        else if(i1->first == to)
            return getLengthImpl(*i0, from) - i0->second;

        return getLengthImpl(*i1, to) - getLengthImpl(*i0, from);
    }

    T getAlphaAtLength(const T& targetLength)
    {
        if(m_LengthDirty)
            updateLength();

        if(targetLength == T(0))
            return T(0);
        else if(targetLength == m_Length)
            return T(1);

        RangeLengthMapContIterType it0, it1;

        for(RangeLengthMapContIterType iter = m_RangeLengthMap.begin(); iter != m_RangeLengthMap.end(); ++iter)
        {
            if(iter->second <= targetLength)
                it0 = iter;

            if(iter->second >= targetLength)
            {
                it1 = iter;

                break;
            }
        }

        // store old length
        T old_length = m_Length;

        // search between it0.first and it1.first for l
        T a_at_length = it0->first;
        T range = it1->first - it0->first;
        T length = it0->second;

        BezierParamType l, r;
        BezierParamType o = sectionImpl(m_Parameter, it0->first, it1->first);

        bool add = true;

        while(range > m_LengthError.m_Epsilon)
        {
            splitAtImpl(o, T(1) / T(2), l, r);

            if(add)
                o = r;
            else
                o = l;

            m_Length = T(0);
            range /= T(2);

            lengthImpl(o, m_LengthError.m_Epsilon, m_LengthCacheDepth);

            if(add)
            {
                length += m_Length;
                a_at_length += range;
            }
            else
            {
                length -= m_Length;
                a_at_length -= range;
            }

            if(std::abs(targetLength - length) <= m_LengthError.m_Epsilon)
                break;

            add = length < targetLength;
        }

        // restore length
        m_Length = old_length;

        return a_at_length;
    }

    void getSection(NGradeBezier& b, const T& from, const T& to)
    {
        if(from < to)
            b.m_Parameter = sectionImpl(m_Parameter, from, to);
        else
            b.m_Parameter = m_Parameter;

        b.updateParameter();
        b.m_LengthDirty = true;
    }

private:
    void copy(const NGradeBezier& from, NGradeBezier& to)
    {
        m_Parameter = from.m_Parameter;
        m_DerivedParameter = from.m_DerivedParameter;
        m_RangeLengthMap = from.m_RangeLengthMap;
        m_LengthVector = from.m_LengthVector;
        m_Length = from.m_Length;
        m_LengthDirty = from.m_LengthDirty;

    }
    void swap(NGradeBezier& from, NGradeBezier& to)
    {
        std::swap(from.m_Parameter, to.m_Parameter);
        std::swap(from.m_DerivedParameter, to.m_DerivedParameter);
        std::swap(from.m_RangeLengthMap, to.m_RangeLengthMap);
        std::swap(from.m_LengthVector, to.m_LengthVector);
        std::swap(from.m_Length, to.m_Length);
        std::swap(from.m_LengthDirty, to.m_LengthDirty);
    }

    void move(NGradeBezier&& from, NGradeBezier& to)
    {
        m_Parameter = std::move(from.m_Parameter);
        m_DerivedParameter = std::move(from.m_DerivedParameter);
        m_RangeLengthMap = std::move(from.m_RangeLengthMap);
        m_LengthVector = std::move(from.m_LengthVector);
        m_Length = std::move(from.m_Length);
        m_LengthDirty = std::move(from.m_LengthDirty);
    }

    void splitAtImpl(const BezierParamType& p, const T& a, BezierParamType& l, BezierParamType& r)
    {
        BezierPointType temp[G][G];

        T a0 = Identities.identityMult<T>() - a;
        T a1 = a;

        for(unsigned int i = 0; i < G; i++)
            temp[0][i] = a0 * p.col(i) + a1 * p.col(i + 1);

        for(unsigned int i = 0; i < G - 1; i++)
            for(unsigned int j = 0; j < G - 1 - i; j++)
                temp[i + 1][j] = a0 * temp[i][j] + a1 * temp[i][j + 1];

        for(unsigned int i = 0; i < G; i++)
        {
            l.col(i + 1) = temp[i][0];
            r.col(i) = temp[G - 1 - i][i];
        }

        l.col(0) = p.col(0);
        r.col(G) = p.col(G);
    }

    BezierParamType sectionImpl(const BezierParamType& p, const T& from, const T& to)
    {
        if(from >= to)
            return p;

        BezierParamType o, l, r;

        o = p;

        splitAtImpl(o, from, l, r);

        o = r;

        splitAtImpl(o, (to - from) / (T(1) - from), l, r);

        o = l;

        return o;
    }

    T getLengthImpl(const RangeLengthType& rangeLength, const T& a)
    {
        // store old length
        T l_org = m_Length;

        // calculate length between rangeLength->first ... a
        m_Length = T(0);

        BezierParamType s = sectionImpl(m_Parameter, rangeLength.first, a);

        lengthImpl(s, m_LengthError.m_Epsilon, m_LengthCacheDepth);

        T length = m_Length;

        // restore 
        m_Length = l_org;

        return length + rangeLength.second;
    }

    template<unsigned int GRADE = G>
    void lengthImpl(const BezierParamType& points, const T& error, size_t depth)
    {
        BezierParamType l, r;

        T len_a = Identities.identityAdd<T>();
        T len_c = Identities.identityAdd<T>();
        T s = Identities.identityMult<T>() / T(2);

        for(unsigned int i = 0; i < G; i++)
            len_a += (points.col(i + 1) - points.col(i)).norm();

        len_c = (points.col(G) - points.col(0)).norm();

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

    void updateLength()
    {
        // clear length related memeber
        m_Length = T(0);
        m_RangeLengthMap.clear();
        m_LengthVector.clear();

        // update length
        m_LengthVector.push_back(T(0));

        lengthImpl(m_Parameter, m_LengthError.m_Epsilon, 0);

        m_LengthVector.push_back(m_Length);

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
            m_RangeLengthMap.insert(std::make_pair(T(1), m_Length));
        }

        m_LengthVector.clear();

        // set length map to undirty
        m_LengthDirty = false;
    }

private:
    bool m_LengthDirty;
    T m_Length;

    RangeLengthMapType m_RangeLengthMap;
    LengthVectoeType m_LengthVector;

    static const size_t m_LengthCacheDepth = 5; // will expand to 2^n + 1 values in m_RangeLengthMap 0, 1/2^n, 2/2^n ... 1
    const size_t m_LengthCacheMax;
    const T m_LengthCacheStep;
};

typedef NGradeBezier<float, 1, 2> Line2F;
typedef NGradeBezier<float, 2, 2> QBezier2F;
typedef NGradeBezier<float, 3, 2> CBezier2F;

typedef NGradeBezier<float, 1, 3> Line3F;
typedef NGradeBezier<float, 2, 3> QBezier3F;
typedef NGradeBezier<float, 3, 3> CBezier3F;

typedef NGradeBezier<double, 1, 2> Line2D;
typedef NGradeBezier<double, 2, 2> QBezier2D;
typedef NGradeBezier<double, 3, 2> CBezier2D;

typedef NGradeBezier<double, 1, 3> Line3D;
typedef NGradeBezier<double, 2, 3> QBezier3D;
typedef NGradeBezier<double, 3, 3> CBezier3D;

#endif //_BASE_NGRADEBEZIER_H_
