#include "HalfEdge2D/Renderables/Spline.h"

#include <cassert>

Spline::Spline()
{

}

Spline::Spline(const Spline& other) : Spline2F(other), RenderableLine(other)
{

}

Spline::~Spline()
{

}
