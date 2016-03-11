#include "HalfEdge2D/HalfEdge2DApplication.h"

#include "HalfEdge2D/HalfEdge2DWidget.h"
#include "HalfEdge2D/HalfEdge2DEventhandler.h"
#include "HalfEdge2D/HalfEdge2DController.h"
#include "HalfEdge2D/HalfEdge2DNavigator.h"

HalfEdge2DApplication::HalfEdge2DApplication(int& argc, char** argv) : QApplication(argc, argv)
{

}

HalfEdge2DApplication::~HalfEdge2DApplication()
{
    delete m_EventHandler;
    delete m_MainWidget;
}

void HalfEdge2DApplication::onRun()
{
    init();
}

void HalfEdge2DApplication::init()
{
    // allocate widget
    m_MainWidget = new HalfEdge2DWidget();
    
    // allocate event handler and add controller and navigator
    m_EventHandler = new HalfEdge2DEventHandler();
    m_EventHandler->addEventInterface(new HalfEdge2DNavigator(m_MainWidget));
    m_EventHandler->addEventInterface(new HalfEdge2DController(m_MainWidget));
    
    m_MainWidget->setEventHandler(m_EventHandler);
    m_MainWidget->show();
}
