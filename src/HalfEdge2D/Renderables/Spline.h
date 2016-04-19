#ifndef _RENDERABLE_SPLINE_H_
#define _RENDERABLE_SPLINE_H_

#include "HalfEdge2D/Base/StaticNGradeBezier.h"

#include "HalfEdge2D/Renderables/RenderableLine.h"

struct SplineSegment
{
    SplineSegment()
    {
        m_Active = false;
    }

    CBezier2F m_Bezier;

    bool m_Active;
};

typedef std::vector<SplineSegment, Eigen::aligned_allocator<SplineSegment>> SplineSegmentVector;

class Spline : public RenderableLine
{
public:
    Spline();
    Spline(const Spline& other);
    ~Spline();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    // getter
    const float& getTension() const;
    const float& getContinuity() const;
    const float& getBias() const;
    const SplineSegmentVector& getSegements();

    std::vector<Vec2f> getPoints() const;
    const Vec2f& getPoint(const size_t& idx) const;
    const bool& isClosed() const;
    
    //setter
    void setTension(const float& tension);
    void setContinuity(const float& continuity);
    void setBias(const float& bias);

    void setPoints(const std::vector<Vec2f>& points);
    void setPoint(const size_t& idx, const Vec2f& point);
    void addPoint(const Vec2f& point);
    void setClosed(const bool& close);
    void removePoint(const size_t& idx);

    size_t getNumControllPoints() const;

private:
    void updateTangentFactors();
    void update();

private:
    SplineSegmentVector m_Segments;
    bool m_Closed;
    float m_Tension;
    float m_Continuity;
    float m_Bias;
    Vec4f m_TangentFactors;
};

#endif //_RENDERABLE_SPLINE_H_
