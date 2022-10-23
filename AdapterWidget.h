#ifndef ADAPTERWIDGET_H
#define ADAPTERWIDGET_H
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <qopengl.h>
class AdapterWidget : public QOpenGLWidget
{
public:
    explicit AdapterWidget(QWidget *parent = nullptr);
    osgViewer::GraphicsWindow* getGraphicsWindow()
    {
        return m_ptrGw.get();
    }

    const osgViewer::GraphicsWindow* getGraphicsWidow()const
    {
        return m_ptrGw.get();
    }
protected:
    virtual void resizeGL(int width, int height);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_ptrGw;
};

#endif // ADAPTERWIDGET_H
