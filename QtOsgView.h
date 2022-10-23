#ifndef QTOSGVIEW_H
#define QTOSGVIEW_H

#include "AdapterWidget.h"

class QtOsgView : public osgViewer::Viewer,public AdapterWidget
{
public:
    explicit QtOsgView(QWidget *parent = nullptr);

    virtual void paintGL();

protected:
    osg::Timer mTimer;			//定时器，控制帧速
    double mStrTime;			//开始tick时间
    double mEndTime;			//结束tick时间

    double mSleepTime;			//需要睡眠的时间
    int num;
};

#endif // QTOSGVIEW_H
