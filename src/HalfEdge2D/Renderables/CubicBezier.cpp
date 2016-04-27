#include "HalfEdge2D/Renderables/CubicBezier.h"

CubicBezier::CubicBezier()
{

}

CubicBezier::CubicBezier(const CubicBezier& other) : CBezier2F(other), RenderableLine(other)
{

}

CubicBezier::CubicBezier(CubicBezier&& other) : CBezier2F(std::move(other)), RenderableLine(std::move(other))
{

}

CubicBezier::~CubicBezier()
{

}
