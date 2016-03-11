#include "HalfEdge2D/HalfEdge2DApplication.h"

#include "HalfEdge2D/HalfEdge2DWidget.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"
#include "HalfEdge2D/HalfEdge/HESBuilder.h"

HalfEdge2DApplication::HalfEdge2DApplication(int& argc, char** argv) : QApplication(argc, argv)
{
    m_Mesh = new HESMesh();

    m_Mesh->addVertex();
    m_Mesh->addVertex();
    m_Mesh->addVertex();
    m_Mesh->addVertex();
    m_Mesh->addVertex();

    m_Mesh->addTriangle(0, 1, 2);
    m_Mesh->addTriangle(0, 2, 3);
    m_Mesh->addTriangle(4, 3, 2);

    HESBuilder builder(m_Mesh);
    builder.build();
}

HalfEdge2DApplication::~HalfEdge2DApplication()
{
    m_MainWidget->deleteLater();
}

void HalfEdge2DApplication::onRun()
{
    init();
}

void HalfEdge2DApplication::init()
{
    m_MainWidget = new HalfEdge2DWidget();
    m_MainWidget->show();
}
