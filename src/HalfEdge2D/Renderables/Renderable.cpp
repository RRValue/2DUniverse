#include "HalfEdge2D/Renderables/Renderable.h"

Renderable::Renderable()
{
    m_Colour= Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
    m_Visible = true;
}

Renderable::Renderable(const Renderable& other) : m_Colour(other.m_Colour), m_Visible(other.m_Visible)
{

}

Renderable::~Renderable()
{

}

const Vec4f& Renderable::getColour() const
{
    return m_Colour;
}

const bool& Renderable::isVisible() const
{
    return m_Visible;
}

void Renderable::setColour(const Vec4f& colour)
{
    m_Colour = colour;
}

void Renderable::setVisible(const bool& visible)
{
    m_Visible = visible;
}
