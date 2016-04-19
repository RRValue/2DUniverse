#include "HalfEdge2D/Renderables/Spline.h"

#include <cassert>

Spline::Spline()
{
    m_Tension = 0.0f;
    m_Continuity = 0.0f;
    m_Bias = 0.0f;
    m_TangentFactors = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
    m_Closed = false;
}

Spline::Spline(const Spline& other) : RenderableLine(other)
{
    m_Tension = other.m_Tension;
    m_Continuity = other.m_Continuity;
    m_Bias = other.m_Bias;
    m_Segments = other.m_Segments;
    m_Closed = other.m_Closed;
}

Spline::~Spline()
{

}

const float& Spline::getTension() const
{
    return m_Tension;
}

const float& Spline::getContinuity() const
{
    return m_Continuity;
}

const float& Spline::getBias() const
{
    return m_Bias;
}

std::vector<Vec2f> Spline::getPoints() const
{
    std::vector<Vec2f> result;

    for(const auto& seg : m_Segments)
        result.push_back(seg.m_Bezier.getPoint(0));

    return result;
}

const Vec2f& Spline::getPoint(const size_t& idx) const
{
    assert(idx >= 0 && idx < m_Segments.size() && "Spline::getPoint: idx out of range");

    return m_Segments[idx].m_Bezier.getPoint(0);
}

const bool& Spline::isClosed() const
{
    return m_Closed;
}

void Spline::setTension(const float& tension)
{
    if(tension == m_Tension)
        return;

    m_Tension = tension;

    if(m_Tension < -1.0f)
        m_Tension = -1.0f;

    if(m_Tension > 1.0f)
        m_Tension = 1.0f;

    updateTangentFactors();
}

void Spline::setContinuity(const float& continuity)
{
    if(continuity == m_Continuity)
        return;

    m_Continuity = continuity;

    if(m_Continuity < -1.0f)
        m_Continuity = -1.0f;

    if(m_Continuity > 1.0f)
        m_Continuity = 1.0f;

    updateTangentFactors();
}

void Spline::setBias(const float& bias)
{
    if(bias == m_Bias)
        return;

    m_Bias = bias;

    if(m_Bias < -1.0f)
        m_Bias = -1.0f;

    if(m_Bias > 1.0f)
        m_Bias = 1.0f;

    updateTangentFactors();
}

const SplineSegmentVector& Spline::getSegements()
{
    return m_Segments;
}

void Spline::setPoints(const std::vector<Vec2f>& points)
{
    m_Segments.clear();

    for(const auto& p : points)
    {
        m_Segments.push_back(SplineSegment());
        m_Segments.back().m_Bezier.setPoint(0, p);
    }

    update();
}

void Spline::setPoint(const size_t& idx, const Vec2f& point)
{
    assert(idx >= 0 && idx < m_Segments.size() && "Spline::setPoint: idx out of range");

    m_Segments[idx].m_Bezier.setPoint(0, point);

    update();
}

void Spline::addPoint(const Vec2f& point)
{
    m_Segments.push_back(SplineSegment());
    m_Segments.back().m_Bezier.setPoint(0, point);

    update();
}

void Spline::setClosed(const bool& close)
{
    if(m_Segments.size() < 3)
        return;

    if(m_Closed == close)
        return;

    m_Closed = close;

    update();
}

void Spline::removePoint(const size_t& idx)
{
    assert(idx >= 0 && idx < m_Segments.size() && "Spline::removePoint: idx out of range");

    m_Segments.erase(m_Segments.begin() + idx);

    if(m_Segments.size() < 3)
        m_Closed = false;

    update();
}

size_t Spline::getNumControllPoints() const
{
    return m_Segments.size();
}

void Spline::updateTangentFactors()
{
    m_TangentFactors(0) = (1.0f - m_Tension) * (1.0f - m_Continuity) * (1.0f + m_Bias);
    m_TangentFactors(1) = (1.0f - m_Tension) * (1.0f + m_Continuity) * (1.0f - m_Bias);
    m_TangentFactors(2) = (1.0f - m_Tension) * (1.0f + m_Continuity) * (1.0f + m_Bias);
    m_TangentFactors(3) = (1.0f - m_Tension) * (1.0f - m_Continuity) * (1.0f - m_Bias);

    m_TangentFactors /= 2.0f;
}

void Spline::update()
{
    const size_t& num_seg = m_Segments.size();

    // activate segments
    if(num_seg > 1)
        for(size_t i = 0; i < num_seg - 1; i++)
            m_Segments[i].m_Active = true;

    if(m_Closed)
        m_Segments[num_seg - 1].m_Active = true;

    // if num_seg <= 1 -> no spline is defined
    if(num_seg <= 1)
        return;

    // set endpoints
    for(size_t i = 0; i < num_seg; i++)
        if(i < num_seg || m_Closed)
            m_Segments[i].m_Bezier.setPoint(3, m_Segments[(i + 1) % num_seg].m_Bezier.getPoint(0));

    // calculate tangents
    size_t idxp, idxn;
    Vec2f p0, p1, p2, p3;
    Vec2f pp0, pp1;
    Vec2f t0, t1;

    for(size_t i = 0; i < num_seg; i++)
    {
        idxp = (i == 0) ? (num_seg - 1) : ( i - 1);
        idxn = (i == num_seg - 1) ? (0) : ( i + 1);

        p0 = m_Segments[idxp].m_Bezier.getPoint(0);
        p1 = m_Segments[i   ].m_Bezier.getPoint(0);
        p2 = m_Segments[idxn].m_Bezier.getPoint(0);
        p3 = m_Segments[i   ].m_Bezier.getPoint(3);

        if(i == 0 && !m_Closed)
        {
            pp0 = p1;
            pp1 = p2 - p1;
        }
        else if(i == num_seg - 1 && !m_Closed)
        {
            pp0 = p1 - p0;
            pp1 = -p1;
        }
        else
        {
            pp0 = p1 - p0;
            pp1 = p2 - p1;
        }

        t0 = (m_TangentFactors[0] * pp0) + (m_TangentFactors[1] * pp1);
        t1 = (m_TangentFactors[2] * pp0) + (m_TangentFactors[3] * pp1);

        m_Segments[i].m_Bezier.setPoint(1, p1 + (t0 / 3.0f));
        m_Segments[i].m_Bezier.setPoint(2, p3 - (t1 / 3.0f));
    }
}
