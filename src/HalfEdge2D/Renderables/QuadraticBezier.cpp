#include "HalfEdge2D/Renderables/QuadraticBezier.h"

QuadraticBezier::QuadraticBezier()
{

}

QuadraticBezier::QuadraticBezier(const QuadraticBezier& other) : QBezier2F(other), RenderableLine(other)
{

}

QuadraticBezier::QuadraticBezier(QuadraticBezier&& other) : QBezier2F(std::move(other)), RenderableLine(std::move(other))
{

}

QuadraticBezier::~QuadraticBezier()
{

}
