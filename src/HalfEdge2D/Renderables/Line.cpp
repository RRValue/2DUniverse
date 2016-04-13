#include "HalfEdge2D/Renderables/Line.h"

#include "HalfEdge2D/Base/MathDefines.h"

Line::Line()
{
    m_Thickness = 0.01f;
    m_Start = Vec2f(-1.0f, 0.0f);
    m_End = Vec2f(1.0f, 0.0f);
    m_Colour= Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
    m_Visible = true;
}

Line::~Line()
{

}

const float& Line::getThickness() const
{
    return m_Thickness;
}

const Vec2f& Line::getPositionStart() const
{
    return m_Start;
}

const Vec2f& Line::getPositionEnd() const
{
    return m_End;
}

const Vec4f& Line::getColour() const
{
    return m_Colour;
}

const bool& Line::isVisible() const
{
    return m_Visible;
}

void Line::setThickness(const float& thickness)
{
    m_Thickness = thickness;
}

void Line::setPositionStart(const Vec2f& pos)
{
    m_Start = pos;
}

void Line::setPositionEnd(const Vec2f& pos)
{
    m_End = pos;
}

void Line::setColour(const Vec4f& colour)
{
    m_Colour = colour;
}

void Line::setVisible(const bool& visible)
{
    m_Visible = visible;
}

Vec2f Line::intersect(const Line& other, bool* const intersect) const
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

    // find root y compontent (cuts with x axis)

    //bool solved;
    //float alpha = rootX(solved);

    Result res = solve({&(float)l1p0[1], &(float)l1p1[1]});

    bool solved = res.m_Solutions == 1;
    float alpha = res[0];

    Vec2f cut_pos(0.0f, 0.0f);

    if(!solved)
        return cut_pos;

    // alpha must be between 0.0 and 1.0 -> we have a cut within the target line
    if(alpha < 0.0f || alpha > 1.0f)
        return cut_pos;

    //cut_pos = other.getRelativePoint(alpha);
    cut_pos = blend({l1p0, l1p1}, alpha);

    // cut_pos.x must be between 0.0 and lenght of source line
    if(cut_pos.x() < 0.0f || cut_pos.x() > l)
        return cut_pos;

    if(intersect != nullptr)
        *intersect = true;

    cut_pos = (m_r_inv * cut_pos) + t;

    return cut_pos;
}

Vec2f Line::getRelativePoint(const float& pos) const
{
    return blend({m_Start, m_End}, pos);
}

bool Line::collinearTo(const Line& l) const
{
    Vec2f nf0 = (m_End - m_Start).normalized();
    Vec2f nf1 = (l.m_End - l.m_Start).normalized();

    Vec2d n0((double)nf0[0], (double)nf0[1]);
    Vec2d n1((double)nf1[0], (double)nf1[1]);
    
    double res = (n0[0] * n1[0]) + (n0[1] * n1[1]);

    return std::abs(1.0 - std::abs(res)) < 1e-5;
}

float Line::getLength() const
{
    return (m_Start - m_End).norm();
}

Vec2f Line::getNormal() const
{
    return (m_Start - m_End).normalized();
}
