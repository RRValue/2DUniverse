#include "HalfEdge2D/Scene/Canvas.h"

Canvas::Canvas()
{
    m_Size = QSize(1.0f, 1.0f);
    m_AspectRatio = 1.0f;
}

Canvas::~Canvas()
{

}

const QSize& Canvas::getSize() const
{
    return m_Size;
}

const float& Canvas::getAspectRatio() const
{
    return m_AspectRatio;
}

void Canvas::setSize(const QSize& size)
{
    m_Size = size;
    m_AspectRatio = float(m_Size.height()) / float(m_Size.width());
}
