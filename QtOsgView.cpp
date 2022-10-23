#include "QtOsgView.h"
#include <QDebug>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>
QtOsgView::QtOsgView(QWidget *parent)
    : AdapterWidget{parent}
{
    getCamera()->setViewport(new osg::Viewport(0, 0, width(), height()));
    getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width()) / static_cast<double>(height()), 1.0f, 10000.0f);
    getCamera()->setGraphicsContext(getGraphicsWindow());
    setThreadingModel(osgViewer::Viewer::SingleThreaded);
    setCameraManipulator(new osgGA::TrackballManipulator);
    addEventHandler(new osgViewer::WindowSizeHandler);
    osgViewer::StatsHandler* pStatsHandler = new osgViewer::StatsHandler;
    pStatsHandler->setKeyEventTogglesOnScreenStats(osgGA::GUIEventAdapter::KEY_F7);
    this->addEventHandler(pStatsHandler);

    mStrTime = 0.0;
    mEndTime = 0.0;
    mSleepTime = 0.0;

    num = 0;

}
void QtOsgView::paintGL()
{
    mStrTime = mTimer.tick();
    frame();
    num++;
    mEndTime = mTimer.tick();

    //计算需要睡眠的时间
    mSleepTime = 1.0 / 60.0 - mTimer.delta_s(mStrTime, mEndTime);

    if (mSleepTime < 0)
    {
        mSleepTime = 0.0;
    }

    //睡眠
    OpenThreads::Thread::microSleep(mSleepTime * 1000000);		//微秒

    //递归调用
    update();
}
