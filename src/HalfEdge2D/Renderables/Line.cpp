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

Vec2f Line::pointAt(const float& pos) const
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

void Line::transform(const Mat3f& m)
{
    Vec3f s = m * Vec3f(m_Start[0], m_Start[1], 1.0f);
    Vec3f e = m * Vec3f(m_End[0], m_End[1], 1.0f);

    m_Start = Vec2f(s[0], s[1]);
    m_End = Vec2f(e[0], e[1]);
}

Line Line::transformed(const Mat3f& m) const
{
    Vec3f s = m * Vec3f(m_Start[0], m_Start[1], 1.0f);
    Vec3f e = m * Vec3f(m_End[0], m_End[1], 1.0f);

    Line line;

    line.m_Start = Vec2f(s[0], s[1]);
    line.m_End = Vec2f(e[0], e[1]);

    return line;
}

Mat3f Line::getOrthoBaseMatrix() const
{
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

    Mat3f m_r;
    Mat3f m_t;

    m_r <<
        std::cos(-a), -std::sin(-a), 0.0f,
        std::sin(-a),  std::cos(-a), 0.0f,
                0.0f,          0.0f, 1.0f;

    m_t <<
        1.0f, 0.0f, -t[0],
        0.0f, 1.0f, -t[1],
        0.0f, 0.0f,  1.0f;

    return m_r * m_t;
}

std::vector<float> Line::rootsX() const
{
    Result solution = solve({&(float)m_Start[0], &(float)m_End[0]});
    std::vector<float> roots;
    
    for(size_t i = 0; i < solution.m_Solutions; i++)
        roots.push_back(solution[0]);

    return roots;
}

std::vector<float> Line::rootsY() const
{
    Result solution = solve({&(float)m_Start[1], &(float)m_End[1]});
    std::vector<float> roots;

    for(size_t i = 0; i < solution.m_Solutions; i++)
        roots.push_back(solution[0]);

    return roots;
}

Vec2fVec Line::intersect(const Line& other) const
{
    Mat3f trans = getOrthoBaseMatrix();

    Line t_line = other.transformed(trans);

    // find root y compontent (cuts with x axis)
    // bool solved;
    // float alpha = rootX(solved);

    Vec2fVec results;
    std::vector<float> root = t_line.rootsY();

    if(root.empty())
        return results;

    float alpha = root[0];

    // alpha must be between 0.0 and 1.0 -> we have a cut within the target line
    if(alpha < 0.0f || alpha > 1.0f)
        return results;

    //cut_pos = other.getRelativePoint(alpha);
    Vec2f cut_pos = t_line.pointAt(alpha);

    // cut_pos.x must be between 0.0 and lenght of source line
    if(cut_pos.x() < 0.0f || cut_pos.x() > getLength())
        return results;

    results.push_back(other.pointAt(alpha));
    
    return results;
}

Vec2fVec Line::intersect(const QuadraticBezier& b) const
{
    Mat3f trans = getOrthoBaseMatrix();

    QuadraticBezier t_bezier = b.transformed(trans);

    // find root y compontent (cuts with x axis)
    // bool solved;
    // float alpha = rootX(solved);

    Vec2fVec results;
    std::vector<float> roots = t_bezier.rootsY();

    if(roots.empty())
        return results;

    for(const auto& alpha : roots)
    {
        // alpha must be between 0.0 and 1.0 -> we have a cut within the target line
        if(alpha < 0.0f || alpha > 1.0f)
            return results;

        //cut_pos = other.getRelativePoint(alpha);
        Vec2f cut_pos = t_bezier.pointAt(alpha);

        // cut_pos.x must be between 0.0 and lenght of source line
        if(cut_pos.x() < 0.0f || cut_pos.x() > getLength())
            return results;

        results.push_back(b.pointAt(alpha));
    }

    return results;
}

Vec2fVec Line::intersect(const CubicBezier& b) const
{
    Mat3f trans = getOrthoBaseMatrix();

    CubicBezier t_bezier = b.transformed(trans);

    // find root y compontent (cuts with x axis)
    // bool solved;
    // float alpha = rootX(solved);

    Vec2fVec results;
    std::vector<float> roots = t_bezier.rootsY();

    if(roots.empty())
        return results;

    for(const auto& alpha : roots)
    {
        // alpha must be between 0.0 and 1.0 -> we have a cut within the target line
        if(alpha < 0.0f || alpha > 1.0f)
            return results;

        //cut_pos = other.getRelativePoint(alpha);
        Vec2f cut_pos = t_bezier.pointAt(alpha);

        // cut_pos.x must be between 0.0 and lenght of source line
        if(cut_pos.x() < 0.0f || cut_pos.x() > getLength())
            return results;

        results.push_back(b.pointAt(alpha));
    }

    return results;
}
