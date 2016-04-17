#include "HalfEdge2D/Renderables/Line.h"

#include "HalfEdge2D/Base/MathDefines.h"

Line::Line()
{
    m_Thickness = 0.01f;
}

Line::Line(const Line& other) : Line2F(other), Renderable(other), m_Thickness(other.m_Thickness)
{

}

Line::~Line()
{

}

const float& Line::getThickness() const
{
    return m_Thickness;
}

void Line::setThickness(const float& thickness)
{
    m_Thickness = thickness;
}

bool Line::collinearTo(const Line& l) const
{
    Vec2f nf0 = (getPoint(1) - getPoint(0)).normalized();
    Vec2f nf1 = (l.getPoint(1) - l.getPoint(0)).normalized();

    Vec2d n0((double)nf0[0], (double)nf0[1]);
    Vec2d n1((double)nf1[0], (double)nf1[1]);
    
    double res = (n0[0] * n1[0]) + (n0[1] * n1[1]);

    return std::abs(1.0 - std::abs(res)) < 1e-5;
}

float Line::getLength() const
{
    return (getPoint(0) - getPoint(1)).norm();
}

Vec2f Line::getNormal() const
{
    return (getPoint(0) - getPoint(1)).normalized();
}

Mat3f Line::getOrthoBaseMatrix() const
{
    Vec2f t = getPoint(0);
    Vec2f p = getPoint(1) - t;

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

Vec2fVec Line::intersect(const Line& other) const
{
    Mat3f trans = getOrthoBaseMatrix();

    Line t_line(other); 
    
    t_line.transform(trans);

    // find root y compontent (cuts with x axis)
    // bool solved;
    // float alpha = rootX(solved);

    Vec2fVec results;
    Line::Roots root = t_line.componentRoots(1);

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

    QuadraticBezier t_bezier(b);

    t_bezier.transform(trans);

    // find root y compontent (cuts with x axis)
    // bool solved;
    // float alpha = rootX(solved);

    Vec2fVec results;
    QuadraticBezier::Roots roots = t_bezier.componentRoots(1);

    if(roots.empty())
        return results;

    for(unsigned int i = 0; i < roots.m_Solutions; i++)
    {
        float alpha = roots[i];

        // alpha must be between 0.0 and 1.0 -> we have a cut within the target line
        if(alpha < 0.0f || alpha > 1.0f)
            continue;

        //cut_pos = other.getRelativePoint(alpha);
        Vec2f cut_pos = t_bezier.pointAt(alpha);

        // cut_pos.x must be between 0.0 and lenght of source line
        if(cut_pos.x() < 0.0f || cut_pos.x() > getLength())
            continue;

        results.push_back(b.pointAt(alpha));
    }

    return results;
}

Vec2fVec Line::intersect(const CubicBezier& b) const
{
    Mat3f trans = getOrthoBaseMatrix();

    CubicBezier t_bezier(b);
    t_bezier.transform(trans);

    // find root y compontent (cuts with x axis)
    // bool solved;
    // float alpha = rootX(solved);

    Vec2fVec results;
    CubicBezier::Roots roots = t_bezier.componentRoots(1);

    if(roots.empty())
        return results;

    for(unsigned int i = 0; i < roots.m_Solutions; i++)
    {
        float alpha = roots[i];

        // alpha must be between 0.0 and 1.0 -> we have a cut within the target line
        if(alpha < 0.0f || alpha > 1.0f)
            continue;

        //cut_pos = other.getRelativePoint(alpha);
        Vec2f cut_pos = t_bezier.pointAt(alpha);

        // cut_pos.x must be between 0.0 and lenght of source line
        if(cut_pos.x() < 0.0f || cut_pos.x() > getLength())
            continue;

        results.push_back(b.pointAt(alpha));
    }

    return results;
}
