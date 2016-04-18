#include "HalfEdge2D/Renderables/CubicBezier.h"

CubicBezier::CubicBezier()
{

}

CubicBezier::CubicBezier(const CubicBezier& other) : CBez2F(other), RenderableLine(other)
{

}

CubicBezier::~CubicBezier()
{

}
