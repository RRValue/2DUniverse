#include "HalfEdge2D/Rendering/QPaintTarget.h"

#include <QtCore/QFile>

#include <QtGui/QPixmap>

QPaintTarget::QPaintTarget(int width, int height)
{
    m_Image = new QPixmap(width, height);
}

QPaintTarget::~QPaintTarget()
{
    delete m_Image;
}

QPaintDevice* const QPaintTarget::getPaintDevice() const
{
    return m_Image;
}

void QPaintTarget::render()
{

}

void QPaintTarget::resize(const int& width, const int& height)
{
    if(m_Image != nullptr)
        delete m_Image;

    m_Image = new QPixmap(width, height);

    setSize(QSize(width, height));
}

void QPaintTarget::save(const QString& fileName)
{
    QFile file(fileName);
    
    if(file.exists())
        file.remove();

    m_Image->save(fileName);
}
