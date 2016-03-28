#include "HalfEdge2D/Base/LineSegment.h"

LineSegment::LineSegment()
{
    m_Start = Vec2f(-1.0f, 0.0);
    m_End = Vec2f(1.0f, 0.0);

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
    m_Min = Vec2f(std::max(m_Start[0], m_End[0]), std::max(m_Start[1], m_End[1]));
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

Vec2f LineSegment::getNormal()
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
    return m_Start + m_Direction.normalized() * pos;
}

bool LineSegment::collinearTo(const LineSegment& l) const
{
    double x_mul = m_Normal[0] * l.m_Normal[0];
    double y_mul = m_Normal[1] * l.m_Normal[1];

    double norm_add = x_mul + y_mul;

    return std::abs(1.0 - std::abs(norm_add)) < 1e-5;
}

Vec2f LineSegment::intersect(const LineSegment& l, bool* const intersect) const
{
    Vec2f intersec_point(0.0f, 0.0f);

    // check collinear //
    if(collinearTo(l))
    {
        bool intersection = false;

        if(m_Start == l.m_Start && m_End != l.m_End)
        {
            intersection = true;
            intersec_point = m_Start;
        }
        if(m_End == l.m_Start && m_Start!= l.m_End)
        {
            intersection = true;
            intersec_point = m_End;
        }
        else
            intersection = false;

        if(intersect != NULL)
            (*intersect) = intersection;

        return intersec_point;
    }

    // intersect
    Vec2f s0 = m_Start;
    Vec2f s1 = l.m_Start;

    Vec2f e0 = m_End;
    Vec2f e1 = l.m_End;

    float A0 = e0.y() - s0.y();
    float B0 = s0.x() - e0.x();
    float C0 = (A0 * s0.x()) + (B0 * s0.y());

    float A1 = e1.y() - s1.y();
    float B1 = s1.x() - e1.x();
    float C1 = (A1 * s1.x()) + (B1 * s1.y());

    float det = (A0 * B1) - (A1 * B0);

    if(det == 0.0f || fabs(det) < 1e-3f)
    {
        if(intersect != NULL)
            (*intersect) = false;
    }
    else
    {
        float x = (((B1 * C0) - (B0 * C1))) / det;
        float y = (((A0 * C1) - (A1 * C0))) / det;

        intersec_point = Vec2f(x, y);

        if(intersect != 0)
        {
            bool is_greater =
                (((x - m_Min.x()) > 0.0f) || ((m_Min.x() - x) < 1e-3f)) &&
                (((y - m_Min.y()) > 0.0f) || ((m_Min.y() - y) < 1e-3f));

            bool is_smaller =
                (((m_Max.x() - x) > 0.0f) || ((x - m_Max.x()) < 1e-3f)) &&
                (((m_Max.y() - y) > 0.0f) || ((y - m_Max.y()) < 1e-3f));

            bool is_greater_l =
                (((x - l.m_Min.x()) > 0.0f) || ((l.m_Min.x() - x) < 1e-3f)) &&
                (((y - l.m_Min.y()) > 0.0f) || ((l.m_Min.y() - y) < 1e-3f));

            bool is_smaller_l =
                (((l.m_Max.x() - x) > 0.0f) || ((x - l.m_Max.x()) < 1e-3f)) &&
                (((l.m_Max.y() - y) > 0.0f) || ((y - l.m_Max.y()) < 1e-3f));

            (*intersect) = is_greater && is_smaller && is_greater_l && is_smaller_l;
        }
    }

    return intersec_point;
}
