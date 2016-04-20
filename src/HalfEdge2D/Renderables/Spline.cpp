#include "HalfEdge2D/Renderables/Spline.h"

#include <cassert>

Spline::Spline()
{
    m_Tension = 0.0f;
    m_Continuity = 0.0f;
    m_Bias = 0.0f;
    m_TangentFactors = Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
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
        result.push_back(seg.getPoint(0));

    return result;
}

const Vec2f& Spline::getPoint(const size_t& idx) const
{
    assert(idx >= 0 && idx < m_Segments.size() && "Spline::getPoint: idx out of range");

    return m_Segments[idx].getPoint(0);
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
        m_Segments.back().setPoint(0, p);
    }

    update();
}

void Spline::setPoint(const size_t& idx, const Vec2f& point)
{
    assert(idx >= 0 && idx < m_Segments.size() && "Spline::setPoint: idx out of range");

    m_Segments[idx].setPoint(0, point);

    update();
}

void Spline::addPoint(const Vec2f& point)
{
    m_Segments.push_back(SplineSegment());
    m_Segments.back().setPoint(0, point);
    m_Segments.back().setVisible(false);

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
            m_Segments[i].setVisible(true);

    if(m_Closed)
        m_Segments[num_seg - 1].setVisible(true);

    // if num_seg <= 1 -> no spline is defined
    if(num_seg <= 1)
        return;

    // set endpoints
    for(size_t i = 0; i < num_seg; i++)
        if(i < num_seg || m_Closed)
            m_Segments[i].setPoint(3, m_Segments[(i + 1) % num_seg].getPoint(0));

    // calculate tangents
    size_t idx0, idx1, idx2;
    Vec2f p0, p1, p2;
    Vec2f seg_p0, seg_p3;
    Vec2f pp0, pp1;
    Vec2f t;

    for(size_t i = 0; i < num_seg; i++)
    {
        idx0 = (i == 0) ? (num_seg - 1) : (i - 1);
        idx1  = i;
        idx2 = (i == num_seg - 1) ? (0) : (i + 1);

        seg_p0 = m_Segments[i].getPoint(0);
        seg_p3 = m_Segments[i].getPoint(3);

        for(size_t j = 0; j < 2; j++)
        {
            idx0 = (idx0 + j) % num_seg;
            idx1 = (idx1 + j) % num_seg;
            idx2 = (idx2 + j) % num_seg;
            
            p0 = m_Segments[idx0].getPoint(0);
            p1 = m_Segments[idx1].getPoint(0);
            p2 = m_Segments[idx2].getPoint(0);

            if(idx1 == 0 && !m_Closed)
            {
                pp0 = p1;
                pp1 = p2 - p1;
            }
            else if(idx1 == num_seg - 1 && !m_Closed)
            {
                pp0 = p1 - p0;
                pp1 = -p1;
            }
            else
            {
                pp0 = p1 - p0;
                pp1 = p2 - p1;
            }

            t = (m_TangentFactors[(j * 2) + 0] * pp0) + (m_TangentFactors[(j * 2) + 1] * pp1);

            if(j == 0)
                t = seg_p0 + (t / 3.0f);
            else
                t = seg_p3 - (t / 3.0f);

            m_Segments[i].setPoint(j + 1, t);
        }
    }
}
