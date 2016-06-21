#ifndef _BASE_SPLINE_H_
#define _BASE_SPLINE_H_

#include "HalfEdge2D/Base/NGradeBezier.h"

template<typename T, unsigned int D>
class NSpline
{
public:
    struct Segment
    {
        Segment()
        {
            m_Active = false;
        }

        Segment(const Segment& other) : m_Active(other.m_Active), m_Bezier(other.m_Bezier)
        {

        }

        Segment(Segment&& other) : m_Active(std::move(other.m_Active)), m_Bezier(std::move(other.m_Bezier))
        {

        }

        Segment& operator=(const Segment& other)
        {
            m_Active = other.m_Active;
            m_Bezier = other.m_Bezier;

            return *this;
        }

        Segment& operator=(Segment&& other)
        {
            m_Active = std::move(other.m_Active);
            m_Bezier = std::move(other.m_Bezier);

            return *this;
        }

        NGradeBezier<T, 3, D> m_Bezier;
        bool m_Active;
    };

    typedef std::vector<Segment, Eigen::aligned_allocator<Segment>> SegmentVector;

private:
    typedef Eigen::Matrix<T, D, 1> SplinePointType;
    typedef std::vector<SplinePointType, Eigen::aligned_allocator<SplinePointType>> SplinePointVectorType;
    
    typedef Eigen::Matrix<T, 4, 1> TangentFactorsType;

    typedef Eigen::Matrix<T, D + 1, 1> TransformPointType;
    typedef Eigen::Matrix<T, D + 1, D + 1> TransformType;

public:
    NSpline()
    {
        m_Tension = T(0);
        m_Continuity = T(0);
        m_Bias = T(0);
        m_TangentFactors = TangentFactorsType(T(0.5), T(0.5), T(0.5), T(0.5));
        m_Closed = false;
        m_LengthDirty = true;
        m_SegmentAlphaStep = T(0);
    }
    NSpline(const NSpline& other)
    {
        m_Tension = other.m_Tension;
        m_Continuity = other.m_Continuity;
        m_Bias = other.m_Bias;
        m_TangentFactors = other.m_TangentFactors;
        m_Segments = other.m_Segments;
        m_Closed = other.m_Closed;
        m_LengthDirty = true;
        m_SegmentAlphaStep = other.m_SegmentAlphaStep;
    }
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    // getter
    const T& getTension() const { return m_Tension; }
    const T& getContinuity() const { return m_Continuity; }
    const T& getBias() const { return m_Bias; }
    const SegmentVector& getSegements() const { return m_Segments; }
    SegmentVector& getSegements() { return m_Segments; }
    const bool& isClosed() const { return m_Closed; }
    size_t getNumControllPoints() const { return m_Segments.size(); }

    SplinePointVectorType getPoints() const
    {
        SplinePointVectorType result;

        for(const auto& seg : m_Segments)
            result.push_back(seg.getPoint(0));

        return result;
    }
    const SplinePointType& getPoint(const size_t& idx) const
    {
        assert(idx >= 0 && idx < m_Segments.size() && "NSpline::getPoint: idx out of range");

        return m_Segments[idx].getPoint(0);
    }

    //setter
    void setTension(const T& tension)
    {
        if(tension == m_Tension)
            return;

        m_Tension = tension;

        if(m_Tension < -T(1))
            m_Tension = -T(1);

        if(m_Tension > T(1))
            m_Tension = T(1);

        updateTangentFactors();
    }
    void setContinuity(const T& continuity)
    {
        if(continuity == m_Continuity)
            return;

        m_Continuity = continuity;

        if(m_Continuity < -T(1))
            m_Continuity = -T(1);

        if(m_Continuity > T(1))
            m_Continuity = T(1);

        updateTangentFactors();
    }
    void setBias(const T& bias)
    {
        if(bias == m_Bias)
            return;

        m_Bias = bias;

        if(m_Bias < -T(1))
            m_Bias = -T(1);

        if(m_Bias > T(1))
            m_Bias = T(1);

        updateTangentFactors();
    }

    void setPoints(const SplinePointVectorType& points)
    {
        m_Segments.clear();

        for(const auto& p : points)
        {
            m_Segments.push_back(SplineSegment());
            m_Segments.back().setPoint(0, p);
        }

        update();
    }
    void setPoint(const size_t& idx, const SplinePointType& point)
    {
        assert(idx >= 0 && idx < m_Segments.size() && "NSpline::setPoint: idx out of range");

        m_Segments[idx].m_Bezier.setPoint(0, point);

        update();
    }
    void addPoint(const SplinePointType& point)
    {
        m_Segments.push_back(Segment());
        m_Segments.back().m_Bezier.setPoint(0, point);
        m_Segments.back().m_Active = false;

        update();
    }
    void setClosed(const bool& close)
    {
        if(m_Segments.size() < 3)
            return;

        if(m_Closed == close)
            return;

        m_Closed = close;

        update();
    }
    void removePoint(const SplinePointType& idx)
    {
        assert(idx >= 0 && idx < m_Segments.size() && "NSpline::removePoint: idx out of range");

        m_Segments.erase(m_Segments.begin() + idx);

        if(m_Segments.size() < 3)
            m_Closed = false;

        update();
    }
    
    SplinePointType pointAt(const T& alpha) const
    {
        if(m_Segments.empty())
            return SplinePointType();

        unsigned int s = 0;
        T a = alpha;

        getSegmentAndFraction(s, a);

        return m_Segments[s].m_Bezier.pointAt(a);
    }

    SplinePointType pointAtL(const T& alpha)
    {
        if(m_Segments.empty())
            return SplinePointType();

        unsigned int s = 0;
        T a = alpha;

        getSegmentAndFractionL(s, a);

        return m_Segments[s].m_Bezier.pointAt(a);
    }

    SplinePointType tangentAt(const T& alpha) const
    {
        if(m_Segments.empty())
            return SplinePointType();

        unsigned int s = 0;
        T a = alpha;

        getSegmentAndFraction(s, a);

        return m_Segments[s].m_Bezier.tangentAt(a);
    }

    SplinePointType tangentAtL(const T& alpha)
    {
        if(m_Segments.empty())
            return SplinePointType();

        unsigned int s = 0;
        T a = alpha;

        getSegmentAndFractionL(s, a);

        return m_Segments[s].m_Bezier.tangentAt(a);
    }

    SplinePointType normalAt(const T& alpha) const
    {
        if(m_Segments.empty())
            return SplinePointType();

        unsigned int s = 0;
        T a = alpha;

        getSegmentAndFraction(s, a);

        return m_Segments[s].m_Bezier.normalAt(a);
    }

    SplinePointType normalAtL(const T& alpha)
    {
        if(m_Segments.empty())
            return SplinePointType();

        unsigned int s = 0;
        T a = alpha;

        getSegmentAndFractionL(s, a);

        return m_Segments[s].m_Bezier.normalAt(a);
    }

    SplinePointType biNormalAt(const T& alpha) const
    {
        if(m_Segments.empty())
            return SplinePointType();

        unsigned int s = 0;
        T a = alpha;

        getSegmentAndFraction(s, a);

        return m_Segments[s].m_Bezier.biNormalAt(a);
    }

    SplinePointType biNormalAtL(const T& alpha)
    {
        if(m_Segments.empty())
            return SplinePointType();

        unsigned int s = 0;
        T a = alpha;

        getSegmentAndFractionL(s, a);

        return m_Segments[s].m_Bezier.biNormalAt(a);
    }

    T curvationAt(const T& alpha) const
    {
        if(m_Segments.empty())
            return T(0);

        unsigned int s = 0;
        T a = alpha;

        getSegmentAndFraction(s, a);

        return m_Segments[s].m_Bezier.curvationAt(a);
    }

    T curvationAtL(const T& alpha) const
    {
        if(m_Segments.empty())
            return T(0);

        unsigned int s = 0;
        T a = alpha;

        getSegmentAndFractionL(s, a);

        return m_Segments[s].m_Bezier.curvationAt(a);
    }

    void transform(const TransformType& m)
    {
        for(auto& s : m_Segments)
            s.m_Bezier.transform(m);

        m_LengthDirty = true;
    }

    T alphaFromSegmentAlpha(unsigned int& seg, T& alpha) const
    {
        return ((T)seg * m_SegmentAlphaStep) + (alpha * m_SegmentAlphaStep);
    }

private:
    void updateTangentFactors()
    {
        m_TangentFactors(0) = (T(1) - m_Tension) * (T(1) - m_Continuity) * (T(1) + m_Bias);
        m_TangentFactors(1) = (T(1) - m_Tension) * (T(1) + m_Continuity) * (T(1) - m_Bias);
        m_TangentFactors(2) = (T(1) - m_Tension) * (T(1) + m_Continuity) * (T(1) + m_Bias);
        m_TangentFactors(3) = (T(1) - m_Tension) * (T(1) - m_Continuity) * (T(1) - m_Bias);

        m_TangentFactors /= T(2);

        update();
    }
    void getSegmentAndFraction(unsigned int& seg, T& alpha) const
    {
        if(m_Closed)
            alpha *= (T)m_Segments.size();
        else
            alpha *= (T)(m_Segments.size() - 1);

        T c = std::floor(alpha);
        
        alpha -= c;
        seg = (unsigned int)c;

        if(m_Closed && seg == m_Segments.size())
            seg = 0;
    }

    void getSegmentAndFractionL(unsigned int& seg, T& alpha)
    {
        if(m_LengthDirty)
            updateLength();

        T l = alpha * m_Length;
        T l_accum = T(0);
        T l0;
        T l1;

        size_t i;
        for(i = 0; i < m_Segments.size(); i++)
        {
            const T& seg_l = m_Segments[i].m_Bezier.getLength();

            l_accum += seg_l;

            if(l_accum <= l)
                continue;

            l1 = l_accum;
            l0 = l_accum - seg_l;

            break;
        }

        seg = i;

        if(m_Closed && seg == m_Segments.size())
        {
            seg = 0;
            alpha = T(0);
        }
        else
            alpha = (l - l0) / (l1 - l0);
    }
    void update()
    {
        m_SegmentAlphaStep = T(0);

        if(m_Segments.empty())
            return;

        const size_t& num_seg = m_Segments.size();

        // activate segments
        if(num_seg > 1)
            for(size_t i = 0; i < num_seg - 1; i++)
                m_Segments[i].m_Active = true;

        m_Segments[num_seg - 1].m_Active = m_Closed;

        // if num_seg <= 1 -> no spline is defined
        if(num_seg <= 1)
            return;

        // update segement alpha step
        if(num_seg > 1 && !m_Closed)
            m_SegmentAlphaStep = T(1) / (T)(num_seg - 1);
        else
            m_SegmentAlphaStep = T(1) / (T)num_seg;

        // set endpoints
        for(size_t i = 0; i < num_seg; i++)
            if(i < num_seg || m_Closed)
                m_Segments[i].m_Bezier.setPoint(3, m_Segments[(i + 1) % num_seg].m_Bezier.getPoint(0));

        // calculate tangents
        size_t idx0, idx1, idx2;
        SplinePointType p0, p1, p2;
        SplinePointType seg_p0, seg_p3;
        SplinePointType pp0, pp1;
        SplinePointType t;

        for(size_t i = 0; i < num_seg; i++)
        {
            idx0 = (i == 0) ? (num_seg - 1) : (i - 1);
            idx1 = i;
            idx2 = (i == num_seg - 1) ? (0) : (i + 1);

            seg_p0 = m_Segments[i].m_Bezier.getPoint(0);
            seg_p3 = m_Segments[i].m_Bezier.getPoint(3);

            for(size_t j = 0; j < 2; j++)
            {
                idx0 = (idx0 + j) % num_seg;
                idx1 = (idx1 + j) % num_seg;
                idx2 = (idx2 + j) % num_seg;

                p0 = m_Segments[idx0].m_Bezier.getPoint(0);
                p1 = m_Segments[idx1].m_Bezier.getPoint(0);
                p2 = m_Segments[idx2].m_Bezier.getPoint(0);

                if(idx1 == 0 && !m_Closed)
                    pp0 = pp1 = p2 - p1;
                else if(idx1 == num_seg - 1 && !m_Closed)
                    pp0 = pp1 = p1 - p0;
                else
                {
                    pp0 = p1 - p0;
                    pp1 = p2 - p1;
                }

                t = (m_TangentFactors[(j * 2) + 0] * pp0) + (m_TangentFactors[(j * 2) + 1] * pp1);

                if(j == 0)
                    t = seg_p0 + (t / T(3));
                else
                    t = seg_p3 - (t / T(3));

                m_Segments[i].m_Bezier.setPoint(j + 1, t);
            }
        }

        m_LengthDirty = true;
    }

    void updateLength()
    {
        m_Length = T(0);

        for(auto& s : m_Segments)
            if(s.m_Active)
                m_Length += s.m_Bezier.getLength();

        m_LengthDirty = false;
    }

private:
    SegmentVector m_Segments;
    bool m_Closed;
    T m_SegmentAlphaStep;
    T m_Tension;
    T m_Continuity;
    T m_Bias;
    TangentFactorsType m_TangentFactors;
    T m_Length;
    bool m_LengthDirty;
};

typedef NSpline<float, 2> Spline2F;
typedef NSpline<float, 3> Spline3F;
typedef NSpline<double, 2> Spline2D;
typedef NSpline<double, 3> Spline3D;

#endif //_BASE_STATICSPLINE_H_
