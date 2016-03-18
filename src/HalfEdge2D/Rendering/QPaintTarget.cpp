#include "HalfEdge2D/Rendering/QPaintTarget.h"

QPaintTarget::QPaintTarget(int width, int height) : QPixmap(width, height)
{
    setSize(QSizeF((qreal)width, (qreal)height));
}

QPaintTarget::~QPaintTarget()
{

}

void QPaintTarget::render()
{

}
