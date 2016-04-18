#include "HalfEdge2D/Renderables/QuadraticBezier.h"

QuadraticBezier::QuadraticBezier()
{

}

QuadraticBezier::QuadraticBezier(const QuadraticBezier& other) : QBez2F(other), RenderableLine(other)
{

}

QuadraticBezier::~QuadraticBezier()
{

}
