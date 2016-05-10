#include "HalfEdge2D/Renderables/Line.h"

#include "HalfEdge2D/Base/MathDefines.h"

#include "HalfEdge2D/Renderables/Circle.h"
#include "HalfEdge2D/Renderables/QuadraticBezier.h"
#include "HalfEdge2D/Renderables/CubicBezier.h"
#include "HalfEdge2D/Renderables/Spline.h"

Line::Line()
{

}

Line::Line(const Line& other) : Line2F(other), RenderableLine(other)
{

}

Line::Line(Line&& other) : Line2F(std::move(other)), RenderableLine(std::move(other))
{

}

Line::~Line()
{

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

IntersectionVector Line::intersect(Circle* const c, const bool& valuesFromIntersector) const
{
    Mat3f trans = getOrthoBaseMatrix();
    Mat3f trans_inv = trans.inverse();

    Circle t_circle(*c);
    Line t_line(*this);

    t_circle.transform(trans);
    t_line.transform(trans);

    const float& r = t_circle.getRadius();
    const Vec2f& p = t_circle.getPosition();
    float l = t_line.getLength();

    IntersectionVector result;

    if(r < p.y())
        return result;

    float length = getLength();

    if(r == p.y())
    {
        if(!valuesFromIntersector)
            result.push_back({p.x() / length, Vec2f(p.x(), 0.0f)});
        else
            result.push_back({0.0f, Vec2f(p.x(), 0.0f)});
    }
    else //r > c.y()
    {
        float a = std::sqrt(std::pow(r, 2.0f) - std::pow(p.y(), 2.0f));

        std::array<float, 2> cuts_x = 
        {
            p.x() + a, 
            p.x() - a
        };

        for(const auto& cut_x : cuts_x)
        {
            if(cut_x < 0.0f || cut_x > l)
                continue;
            
            if(!valuesFromIntersector)
                result.push_back({cut_x / length, Vec2f(cut_x, 0.0f)});
            else
                result.push_back({0.0f, Vec2f(cut_x, 0.0f)});
        }
    }

    for(auto& r : result)
    {
        Vec3f re_t_r = trans_inv * Vec3f(r.m_Point(0), r.m_Point(1), 1.0f);

        r.m_Point(0) = re_t_r(0);
        r.m_Point(1) = re_t_r(1);
    }

    return result;
}

IntersectionVector Line::intersect(Line* const l, const bool& valuesFromIntersector) const
{
    Mat3f trans = getOrthoBaseMatrix();

    Line t_line(*l);

    t_line.transform(trans);

    // find root y compontent (cuts with x axis)

    IntersectionVector results;
    Line::Roots root = t_line.componentRoots(1);

    if(root.empty())
        return results;

    float alpha = root[0];

    // alpha must be between 0.0 and 1.0 -> we have a cut within the target line
    if(alpha < 0.0f || alpha > 1.0f)
        return results;

    Vec2f cut_pos = t_line.pointAt(alpha);

    // cut_pos.x must be between 0.0 and lenght of source line
    float length = getLength();

    if(cut_pos.x() < 0.0f || cut_pos.x() > length)
        return results;

    if(valuesFromIntersector)
        results.push_back({alpha, l->pointAt(alpha)});
    else
        results.push_back({cut_pos.x() / length, l->pointAt(alpha)});

    return results;
}

IntersectionVector Line::intersect(QuadraticBezier* const b, const bool& valuesFromIntersector) const
{
    Mat3f trans = getOrthoBaseMatrix();

    QuadraticBezier t_bezier(*b);

    t_bezier.transform(trans);

    // find root y compontent (cuts with x axis)

    IntersectionVector results;
    QuadraticBezier::Roots roots = t_bezier.componentRoots(1);

    if(roots.empty())
        return results;

    float length = getLength();

    for(unsigned int i = 0; i < roots.m_Solutions; i++)
    {
        float alpha = roots[i];

        // alpha must be between 0.0 and 1.0 -> we have a cut within the target line
        if(alpha < 0.0f || alpha > 1.0f)
            continue;

        Vec2f cut_pos = t_bezier.pointAt(alpha);

        // cut_pos.x must be between 0.0 and lenght of source line
        if(cut_pos.x() < 0.0f || cut_pos.x() > length)
            continue;

        if(valuesFromIntersector)
            results.push_back({alpha, b->pointAt(alpha)});
        else
            results.push_back({cut_pos.x() / length, b->pointAt(alpha)});
    }

    return results;
}

IntersectionVector Line::intersect(CubicBezier* const b, const bool& valuesFromIntersector) const
{
    Mat3f trans = getOrthoBaseMatrix();

    CubicBezier t_bezier(*b);
    t_bezier.transform(trans);

    // find root y compontent (cuts with x axis)
    IntersectionVector results;
    CubicBezier::Roots roots = t_bezier.componentRoots(1);

    if(roots.empty())
        return results;

    float length = getLength();

    for(unsigned int i = 0; i < roots.m_Solutions; i++)
    {
        float alpha = roots[i];

        // alpha must be between 0.0 and 1.0 -> we have a cut within the target line
        if(alpha < 0.0f || alpha > 1.0f)
            continue;

        Vec2f cut_pos = t_bezier.pointAt(alpha);

        // cut_pos.x must be between 0.0 and lenght of source line
        if(cut_pos.x() < 0.0f || cut_pos.x() > length)
            continue;

        if(valuesFromIntersector)
            results.push_back({alpha, b->pointAt(alpha)});
        else
            results.push_back({cut_pos.x() / length, b->pointAt(alpha)});
    }

    return results;
}

IntersectionVector Line::intersect(Spline* const s, const bool& valuesFromIntersector) const
{
    Mat3f trans = getOrthoBaseMatrix();
    Mat3f trans_inv = trans.inverse();

    Spline t_spline(*s);
    t_spline.transform(trans);

    IntersectionVector results;
    const Spline::SegmentVector& segements = s->getSegements();
    const Spline::SegmentVector& t_segements = t_spline.getSegements();

    float length = getLength();

    for(size_t i = 0; i < t_spline.getNumControllPoints(); i++)
    {
        if(!segements[i].m_Active)
            continue;

        // find root y compontent (cuts with x axis)
        CubicBezier::Roots seg_roots = t_segements[i].m_Bezier.componentRoots(1);

        if(seg_roots.empty())
            continue;

        for(unsigned int j = 0; j < seg_roots.m_Solutions; j++)
        {
            float alpha = seg_roots[j];

            // alpha must be between 0.0 and 1.0 -> we have a cut within the target line
            if(alpha < 0.0f || alpha > 1.0f)
                continue;

            Vec2f cut_pos = t_segements[i].m_Bezier.pointAt(alpha);

            // cut_pos.x must be between 0.0 and lenght of source line
            if(cut_pos.x() < 0.0f || cut_pos.x() > getLength())
                continue;

            if(valuesFromIntersector)
                results.push_back({alpha, segements[i].m_Bezier.pointAt(alpha)});
            else
                results.push_back({cut_pos.x() / length, segements[i].m_Bezier.pointAt(alpha)});
        }
    }

    return results;
}
