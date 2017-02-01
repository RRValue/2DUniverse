#include "2DUniverse/Rendering/QPaintTarget.h"

#include "2DUniverse/Scene/ViewPort.h"

#include <QtCore/QFile>

#include <QtGui/QImage>
#include <QtGui/QColor>

QPaintTarget::QPaintTarget(int width, int height)
{
    m_Image = new QImage(width, height, QImage::Format_RGB32);
}

QPaintTarget::~QPaintTarget()
{
    delete m_Image;
}

Vec4f QPaintTarget::getColourAtPos(const unsigned int& x, const unsigned int& y)
{
    Vec4f colour;

    if(m_Image == nullptr)
        return colour;

    if(x >= m_Size.width() || y >= m_Size.height())
        return colour;

    QColor pixel = m_Image->pixel(x, y);

    colour[0] = pixel.redF();
    colour[1] = pixel.greenF();
    colour[2] = pixel.blueF();
    colour[3] = 1.0f;

    return colour;
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

    m_Image = new QImage(new_size, QImage::Format_RGB32);

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
