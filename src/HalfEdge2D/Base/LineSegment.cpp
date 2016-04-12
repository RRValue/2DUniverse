#include "HalfEdge2D/Base/LineSegment.h"

#include "HalfEdge2D/Base/MathDefines.h"

LineSegment::LineSegment()
{
    m_Start = Vec2f(-1.0f, 0.0);
    m_End = Vec2f(1.0f, 0.0);

    updateParameter();
}

LineSegment::LineSegment(const Vec2f& start, const Vec2f& end)
{
    m_Start = start;
    m_End = end;

    updateParameter();
}

LineSegment::~LineSegment()
{

}

void LineSegment::updateParameter()
{
    m_Direction = m_End - m_Start;

    setNormalFromDirection(m_Direction);

    m_Lenght = m_Direction.norm();
    m_Min = Vec2f(std::min(m_Start[0], m_End[0]), std::min(m_Start[1], m_End[1]));
    m_Max = Vec2f(std::max(m_Start[0], m_End[0]), std::max(m_Start[1], m_End[1]));
    m_Valid = m_Lenght > 1e-3f;
}

void LineSegment::setNormalFromDirection(const Vec2f& direction)
{
    double dir_x = (double)direction.x();
    double dir_y = (double)direction.y();

    double dir_lenght = sqrt(pow(dir_x, 2.0) + pow(dir_y, 2.0));

    if(fabs(dir_lenght) < 1e-5)
    {
        m_Normal[0] = 0.0;
        m_Normal[1] = 0.0;
    }
    else
    {
        m_Normal[0] = dir_x / dir_lenght;
        m_Normal[1] = dir_y / dir_lenght;
    }
}

const Vec2f& LineSegment::getPositionStart() const
{
    return m_Start;
}

const Vec2f& LineSegment::getPositionEnd() const
{
    return m_End;
}

Vec2f LineSegment::getNormal() const
{
    return Vec2f((float)m_Normal[0], (float)m_Normal[1]);
}

float LineSegment::getLength() const
{
    return m_Lenght;
}

void LineSegment::setStart(const Vec2f& pos)
{
    m_Start = pos;

    updateParameter();
}

void LineSegment::setEnd(const Vec2f& pos)
{
    m_End = pos;

    updateParameter();  
}

Vec2f LineSegment::getRelativePoint(const float& pos) const
{
    return m_Start + (m_Direction * pos);
}

bool LineSegment::collinearTo(const LineSegment& l) const
{
    double x_mul = m_Normal[0] * l.m_Normal[0];
    double y_mul = m_Normal[1] * l.m_Normal[1];

    double norm_add = x_mul + y_mul;

    return std::abs(1.0 - std::abs(norm_add)) < 1e-5;
}

Vec2f LineSegment::intersect(const LineSegment& other, bool* const intersect) const
{
    if(intersect != nullptr)
        *intersect = false;

    Vec2f intersec_point(0.0f, 0.0f);

    const Vec2f& l0p0 = m_Start;
    const Vec2f& l0p1 = m_End;

    Vec2f t = l0p0;
    Vec2f p = l0p1 - t;

    float l = p.norm();
    float a = std::asin(p.y() / l);

    if(p.x() < 0.0f)
        a = PI_F - a;

    if(a < 0.0f)
        a += PI2_F;

    Mat2f m_r;
    Mat2f m_r_inv;

    m_r << std::cos(-a), -std::sin(-a), std::sin(-a), std::cos(-a);
    m_r_inv = m_r.inverse();

    Vec2f l1p0 = m_r * (other.m_Start - t);
    Vec2f l1p1 = m_r * (other.m_End - t);

    // cut:
    // find root of y0 * (1 - a) + y1
    // a = y0 / (y0 - y1)
    // y0 - y1 must be greater than epslion (1e-5f)
    // a must be between 0.0 and 1.0 -> we have a cut

    Vec2f cut_pos(0.0f, 0.0f);
    float alpha = l1p0.y() - l1p1.y();

    if(std::abs(alpha) < 1e-5f)
        return cut_pos;

    alpha = l1p0.y() / alpha;

    if(alpha < 0.0f || alpha > 1.0f)
        return cut_pos;

    cut_pos = m_Blender.blend({l1p0, l1p1}, alpha);

    if(cut_pos.x() < 0.0f || cut_pos.x() > l)
        return cut_pos;

    if(intersect != nullptr)
        *intersect = true;

    cut_pos = (m_r_inv * cut_pos) + t;

    return cut_pos;
}
