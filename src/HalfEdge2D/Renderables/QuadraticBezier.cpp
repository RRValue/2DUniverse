#include "HalfEdge2D/Renderables/QuadraticBezier.h"

QuadraticBezier::QuadraticBezier()
{

}

QuadraticBezier::QuadraticBezier(const QuadraticBezier& other) : QBezier2F(other), RenderableLine(other)
{

}

QuadraticBezier::~QuadraticBezier()
{

}
