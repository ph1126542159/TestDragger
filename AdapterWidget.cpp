#include "AdapterWidget.h"
#include <QMouseEvent>
AdapterWidget::AdapterWidget(QWidget *parent)
    : QOpenGLWidget{parent}
{
    m_ptrGw = new osgViewer::GraphicsWindowEmbedded(0, 0, width(), height());
    setFocusPolicy(Qt::ClickFocus);
}
void AdapterWidget::resizeGL(int width, int height)
{
    m_ptrGw->getEventQueue()->windowResize(0, 0, width, height);
    m_ptrGw->resized(0, 0, width, height);
}
void AdapterWidget::keyPressEvent(QKeyEvent* event)
{
    m_ptrGw->getEventQueue()->keyPress((osgGA::GUIEventAdapter::KeySymbol) *(event->text().toUtf8().data()));
}

void AdapterWidget::keyReleaseEvent(QKeyEvent* event)
{
    m_ptrGw->getEventQueue()->keyRelease((osgGA::GUIEventAdapter::KeySymbol)*(event->text().toUtf8().data()));
}


void AdapterWidget::mousePressEvent(QMouseEvent* event)
{
    int button = 0;
    switch (event->button())
    {
    case(Qt::LeftButton) :
        button = 1;
        break;
    case (Qt::MiddleButton) :
        button = 2;
        break;
    case (Qt::RightButton) :
        button = 3;
        break;
    case (Qt::NoButton) :
        button = 0;
        break;
    default:
        button = 0;
        break;
    }

    m_ptrGw->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
}

void AdapterWidget::mouseReleaseEvent(QMouseEvent* event)
{
    int button = 0;
    switch (event->button())
    {
    case(Qt::LeftButton) :
        button = 1;
        break;
    case(Qt::MiddleButton) :
        button = 2;
        break;
    case(Qt::RightButton) :
        button = 3;
        break;
    case(Qt::NoButton) :
        button = 0;
        break;
    default:
        button = 0;
        break;
    }
    m_ptrGw->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
}


void  AdapterWidget::mouseMoveEvent(QMouseEvent* event)
{
    m_ptrGw->getEventQueue()->mouseMotion(event->x(), event->y());
}

void AdapterWidget::wheelEvent(QWheelEvent* event)
{
    m_ptrGw->getEventQueue()->mouseScroll((event->angleDelta().y()>0) ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN);
}
