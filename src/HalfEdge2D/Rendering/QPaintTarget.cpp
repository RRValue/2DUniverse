#include "HalfEdge2D/Rendering/QPaintTarget.h"

#include "HalfEdge2D/Scene/ViewPort.h"

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
    {
        if(width == m_Image->width() && height == m_Image->height())
            return;

        delete m_Image;
    }

    QSize new_size(width, height);

    m_Image = new QPixmap(new_size);

    setSize(QSize(width, height));

    for(const auto& vp : m_ViewPorts)
        vp->setTargetSize(new_size);
}

void QPaintTarget::save(const QString& fileName)
{
    QFile file(fileName);
    
    if(file.exists())
        file.remove();

    m_Image->save(fileName);
}
