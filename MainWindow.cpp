#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "osgUtil/Optimizer"
#include "osgGA/KeySwitchMatrixManipulator"
#include "osgGA/TrackballManipulator"
#include "firstpersonmanipulator.h"
#include "osg/ShapeDrawable"
#include <QFileDialog>
#include <osgDB/ReadFile>
#include <QDebug>
#include <osg/MatrixTransform>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}
osg::ref_ptr<osg::Node> createQuad(float fSize){
    osg::ref_ptr<osg::Geode> geode=new osg::Geode;
    osg::ref_ptr<osg::Geometry> geom=new osg::Geometry;
    osg::ref_ptr<osg::Vec3Array> v=new osg::Vec3Array;
    v->push_back(osg::Vec3(-fSize,fSize,0.0f));
    v->push_back(osg::Vec3(-fSize,-fSize,0.0f));
    v->push_back(osg::Vec3(fSize,-fSize,0.0f));
    v->push_back(osg::Vec3(fSize,fSize,0.0f));

    geom->setVertexArray(v);
    //创建颜色数组
    osg::ref_ptr<osg::Vec4Array> vc = new osg::Vec4Array();
    //添加数据
    vc->push_back(osg::Vec4(0.22f,0.22f,0.22f,1.0f));
    vc->push_back(osg::Vec4(0.22f,0.22f,0.22f,1.0f));
    vc->push_back(osg::Vec4(0.22f,0.22f,0.22f,1.0f));
    vc->push_back(osg::Vec4(0.22f,0.22f,0.22f,1.0f));

    //设置颜色数组
    geom->setColorArray(vc.get());
    //设置颜色的绑定方式为单个顶点
    geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

    //创建法线数组
    osg::ref_ptr<osg::Vec3Array> nc = new osg::Vec3Array();
    //添加法线
    nc->push_back(osg::Vec3(0.0f,0.0f,1.0f));

    //设置法线数组
    geom->setNormalArray(nc.get());
    //设置法线的绑定方式为全部顶点
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

    //添加图元，绘图基元为四边形
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

    //添加到叶节点
    geode->addDrawable(geom.get());

    return geode;
}

osg::ref_ptr<osg::Geode> createBox(){
    osg::ref_ptr<osg::Geode> geode=new osg::Geode;
    //创建精细度对象，精细度越高，细分就越多
    osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
    //设置精细度为0.5f
    hints->setDetailRatio(0.5f);
    osg::ref_ptr<osg::ShapeDrawable> shape=new osg::ShapeDrawable(new osg::Box(osg::Vec3f(0,0,0),5.0f),hints);
    geode->addDrawable(shape);

    return geode;
}

void MainWindow::initUI()
{
    osg::ref_ptr<osg::Group>root=new osg::Group;
    root->addChild(createQuad(10000.0f));
    osg::ref_ptr<osg::Geode> geode=createBox();
    osg::ref_ptr<osg::MatrixTransform> pp=new osg::MatrixTransform;
    pp->addChild(geode);
    pp->setMatrix(osg::Matrix::translate(osg::Vec3f(0,0,0)));

    root->addChild(pp);
    osgUtil::Optimizer opt;
    opt.optimize(root.get());

    osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keySwitch = new osgGA::KeySwitchMatrixManipulator;
    keySwitch->addMatrixManipulator('1', "Trackball",new osgGA::TrackballManipulator);

    osg::ref_ptr<FirstPersonManipulator> firstPerson=new FirstPersonManipulator();
    keySwitch->addMatrixManipulator('2', "First",firstPerson);
    ui->openGLWidget->setCameraManipulator(keySwitch);
    ui->openGLWidget->setSceneData(root.get());
}


void MainWindow::on_actionPrintCamerPos_triggered()
{
    osgGA::KeySwitchMatrixManipulator* map=static_cast<osgGA::KeySwitchMatrixManipulator*>(ui->openGLWidget->getCameraManipulator());
    osgGA::TrackballManipulator* cam=dynamic_cast<osgGA::TrackballManipulator*>(map->getCurrentMatrixManipulator());
    if(nullptr==cam) {
        return;
    }
    osg::Vec3d eye;
    osg::Quat rota;
    cam->getTransformation(eye,rota);
    qDebug()<<"eye:"<<eye.x()<<eye.y()<<eye.z();
    qDebug()<<"rota:"<<rota.x()<<rota.y()<<rota.z();
}


void MainWindow::on_actionPrintFirtCamerpos_triggered()
{
    osgGA::KeySwitchMatrixManipulator* map=static_cast<osgGA::KeySwitchMatrixManipulator*>(ui->openGLWidget->getCameraManipulator());
    FirstPersonManipulator* cam=dynamic_cast<FirstPersonManipulator*>(map->getCurrentMatrixManipulator());
    if(nullptr==cam) {
        return;
    }
    osg::Vec3 eye;
    osg::Vec3 rota;
    cam->getTransformation(eye,rota);
    qDebug()<<"eye:"<<eye.x()<<eye.y()<<eye.z();
    qDebug()<<"rota:"<<rota.x()<<rota.y()<<rota.z();
}


void MainWindow::on_actionModel_triggered()
{
    QString path=QFileDialog::getOpenFileName(nullptr,QString("打开文件"),"");
    if(path.isEmpty()) return;

    float scale=0.025f;
    osg::ref_ptr<osg::Node> node=osgDB::readNodeFile(path.toStdString());

    osg::ref_ptr<osg::MatrixTransform> mt=new osg::MatrixTransform;
    mt->addChild(node);
    mt->setMatrix(osg::Matrix::rotate(osg::inDegrees(90.0f),osg::X_AXIS,
                                      osg::inDegrees(0.0f),osg::Y_AXIS,
                                      osg::inDegrees(90.0f),osg::Z_AXIS)*
                  osg::Matrix::scale(osg::Vec3f(scale,scale,scale))*
                  osg::Matrix::translate(osg::Vec3f(0,0,node->getBound().radius()*scale*0.75f)));

    ui->openGLWidget->getSceneData()->asGroup()->addChild(mt);
}

