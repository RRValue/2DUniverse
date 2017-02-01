#include "2DUniverse/Renderables/Line.h"

#include "2DUniverse/Base/MathDefines.h"

#include "2DUniverse/Renderables/Circle.h"
#include "2DUniverse/Renderables/QuadraticBezier.h"
#include "2DUniverse/Renderables/CubicBezier.h"
#include "2DUniverse/Renderables/Spline.h"

Line::Line()
{

}

Line::Line(const Line& other) : Line2D(other), RenderableLine(other)
{

}

Line::Line(Line&& other) : Line2D(std::move(other)), RenderableLine(std::move(other))
{

}

Line::~Line()
{

}

bool Line::collinearTo(const Line& l) const
{
    Vec2d nf0 = (getPoint(1) - getPoint(0)).normalized();
    Vec2d nf1 = (l.getPoint(1) - l.getPoint(0)).normalized();

    Vec2d n0((double)nf0[0], (double)nf0[1]);
    Vec2d n1((double)nf1[0], (double)nf1[1]);
    
    double res = (n0[0] * n1[0]) + (n0[1] * n1[1]);

    return std::abs(1.0 - std::abs(res)) < 1e-5;
}

double Line::getLength() const
{
    return (getPoint(0) - getPoint(1)).norm();
}

Vec2d Line::getNormal() const
{
    return (getPoint(0) - getPoint(1)).normalized();
}

Mat3d Line::getOrthoBaseMatrix() const
{
    Vec2d t = getPoint(0);
    Vec2d p = getPoint(1) - t;

    double l = p.norm();
    double a = std::asin(p.y() / l);

    if(p.x() < 0.0)
        a = PI_F - a;

    if(a < 0.0)
        a += PI2_F;

    Mat3d m_r;
    Mat3d m_t;

    m_r <<
        std::cos(-a), -std::sin(-a), 0.0,
        std::sin(-a),  std::cos(-a), 0.0,
                 0.0,           0.0, 1.0;

    m_t <<
        1.0, 0.0, -t[0],
        0.0, 1.0, -t[1],
        0.0, 0.0,   1.0;

    return m_r * m_t;
}

IntersectionVector Line::intersect(Circle* const c, const bool& valuesFromIntersector) const
{
    Mat3d trans = getOrthoBaseMatrix();
    Mat3d trans_inv = trans.inverse();

    double line_angle = -std::acos(trans(0, 0));

    Circle t_circle(*c);
    Line t_line(*this);

    t_circle.transform(trans);
    t_line.transform(trans);

    const float& r = t_circle.getRadius();
    const Vec2d& p = t_circle.getPosition();
    double l = t_line.getLength();

    IntersectionVector result;

    if(r < p.y())
        return result;

    double length = getLength();

    if(r == p.y())
    {
        if(valuesFromIntersector)
            result.push_back({-PIHALF_F, Vec2d(p.x(), 0.0)});
        else
            result.push_back({p.x() / length, Vec2d(p.x(), 0.0)});
    }
    else //r > c.y()
    {
        double a = std::sqrt(std::pow(r, 2.0) - std::pow(p.y(), 2.0));

        std::array<double, 2> cuts_x = 
        {
            p.x() + a, 
            p.x() - a
        };

        for(const auto& cut_x : cuts_x)
        {
            if(cut_x < 0.0 || cut_x > l)
                continue;
            
            if(valuesFromIntersector)
            {
                double angle = -PIHALF_F;
                
                if(cut_x > p.x())
                    angle += std::acos(p.y() / r);
                else
                    angle -= std::acos(p.y() / r);

                result.push_back({angle, Vec2d(cut_x, 0.0)});
            }
            else
                result.push_back({cut_x / length, Vec2d(cut_x, 0.0)});
        }
    }

    for(auto& r : result)
    {
        Vec3d re_t_r = trans_inv * Vec3d(r.m_Point(0), r.m_Point(1), 1.0);

        r.m_Point(0) = re_t_r(0);
        r.m_Point(1) = re_t_r(1);

        r.m_Alpha += line_angle;

        while(r.m_Alpha < 0.0f)
            r.m_Alpha += PI2_F;

        while(r.m_Alpha >= PI2_F)
            r.m_Alpha -= PI2_F;
    }

    return result;
}

IntersectionVector Line::intersect(Line* const l, const bool& valuesFromIntersector) const
{
    IntersectionVector results;

    if(collinearTo(*l))
        return results;

    Mat3d trans = getOrthoBaseMatrix();

    Line t_line(*l);

    t_line.transform(trans);

    // find root y compontent (cuts with x axis)
    Line::Roots root = t_line.componentRoots(1);

    if(root.empty())
        return results;

    double alpha = root[0];

    // alpha must be between 0.0 and 1.0 -> we have a cut within the target line
    if(alpha < 0.0 || alpha > 1.0)
        return results;

    Vec2d cut_pos = t_line.pointAt(alpha);

    // cut_pos.x must be between 0.0 and lenght of source line
    double length = getLength();

    if(cut_pos.x() < 0.0 || cut_pos.x() > length)
        return results;

    if(valuesFromIntersector)
        results.push_back({alpha, l->pointAt(alpha)});
    else
        results.push_back({cut_pos.x() / length, l->pointAt(alpha)});

    return results;
}

IntersectionVector Line::intersect(QuadraticBezier* const b, const bool& valuesFromIntersector) const
{
    Mat3d trans = getOrthoBaseMatrix();

    QuadraticBezier t_bezier(*b);

    t_bezier.transform(trans);

    // find root y compontent (cuts with x axis)

    IntersectionVector results;
    QuadraticBezier::Roots roots = t_bezier.componentRoots(1);

    if(roots.empty())
        return results;

    double length = getLength();

    for(unsigned int i = 0; i < roots.m_Solutions; i++)
    {
        double alpha = roots[i];

        // alpha must be between 0.0 and 1.0 -> we have a cut within the target line
        if(alpha < 0.0 || alpha > 1.0)
            continue;

        Vec2d cut_pos = t_bezier.pointAt(alpha);

        // cut_pos.x must be between 0.0 and lenght of source line
        if(cut_pos.x() < 0.0 || cut_pos.x() > length)
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
    Mat3d trans = getOrthoBaseMatrix();

    CubicBezier t_bezier(*b);
    t_bezier.transform(trans);

    // find root y compontent (cuts with x axis)
    IntersectionVector results;
    CubicBezier::Roots roots = t_bezier.componentRoots(1);

    if(roots.empty())
        return results;

    double length = getLength();

    for(unsigned int i = 0; i < roots.m_Solutions; i++)
    {
        double alpha = roots[i];

        // alpha must be between 0.0 and 1.0 -> we have a cut within the target line
        if(alpha < 0.0 || alpha > 1.0)
            continue;

        Vec2d cut_pos = t_bezier.pointAt(alpha);

        // cut_pos.x must be between 0.0 and lenght of source line
        if(cut_pos.x() < 0.0 || cut_pos.x() > length)
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
    Mat3d trans = getOrthoBaseMatrix();
    Mat3d trans_inv = trans.inverse();

    Spline t_spline(*s);
    t_spline.transform(trans);

    IntersectionVector results;
    const Spline::SegmentVector& segements = s->getSegements();
    const Spline::SegmentVector& t_segements = t_spline.getSegements();

    double length = getLength();

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
            double alpha = seg_roots[j];

            // alpha must be between 0.0 and 1.0 -> we have a cut within the target line
            if(alpha < 0.0 || alpha > 1.0)
                continue;

            Vec2d cut_pos = t_segements[i].m_Bezier.pointAt(alpha);

            // cut_pos.x must be between 0.0 and lenght of source line
            if(cut_pos.x() < 0.0 || cut_pos.x() > getLength())
                continue;

            if(valuesFromIntersector)
                results.push_back({t_spline.alphaFromSegmentAlpha(i, alpha), segements[i].m_Bezier.pointAt(alpha)});
            else
                results.push_back({cut_pos.x() / length, segements[i].m_Bezier.pointAt(alpha)});
        }
    }

    return results;
}
