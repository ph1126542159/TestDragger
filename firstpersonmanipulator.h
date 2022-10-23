#ifndef FIRSTPERSONMANIPULATOR_H
#define FIRSTPERSONMANIPULATOR_H
#include<osgGA/CameraManipulator>
#include<osgViewer/Viewer>

class FirstPersonManipulator:public osgGA::CameraManipulator
{
public:
    FirstPersonManipulator();
    ~FirstPersonManipulator();
    virtual void setByMatrix(const osg::Matrixd& ){};
    virtual void setByInverseMatrix(const osg::Matrixd& ) {};
    virtual osg::Matrixd getMatrix() const;
    virtual osg::Matrixd getInverseMatrix() const;
    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

    osg::Vec3 Screen2World(osgViewer::Viewer* view, float x, float y);
    bool ChangePosition(osg::Vec3 delta);
    void SetStep(int delta_step);

    void getTransformation( osg::Vec3& eye,osg::Vec3& rota);
private:
    void keyboraKeyDown(osgViewer::Viewer* view,const osgGA::GUIEventAdapter &ea);
    void mouseWheelScroll(osgViewer::Viewer* view,const osgGA::GUIEventAdapter &ea);
    void mouseLeftPush(osgViewer::Viewer* view,const osgGA::GUIEventAdapter &ea);
    void mouseRightPush(osgViewer::Viewer* view,const osgGA::GUIEventAdapter &ea);
    void mouseLeftRelease(osgViewer::Viewer* view,const osgGA::GUIEventAdapter &ea);
    void mouseRightRelease(osgViewer::Viewer* view,const osgGA::GUIEventAdapter &ea);
    void mouseLeftDrage(osgViewer::Viewer* view,const osgGA::GUIEventAdapter &ea);
    void mouseRightDrage(osgViewer::Viewer* view,const osgGA::GUIEventAdapter &ea);
    osg::Vec3 screenToWorld(osgViewer::Viewer* viewer,double dx,double dy);
    void rayLinePick(osgViewer::Viewer* view, float x, float y);
    osg::Vec3 m_vPosition;
    osg::Vec3 m_vRotation;
    int m_vStep;
    float m_vRotateStep;
    int m_iLeftX;
    int m_iLeftY;
    bool m_bLeftDown;
    osg::Vec3d m_pickPoint;
    osg::MatrixTransform* m_pickObj;
    bool _isDragging;
    bool m_bRightDown;
};

#endif // FIRSTPERSONMANIPULATOR_H
