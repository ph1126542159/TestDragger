#include "firstpersonmanipulator.h"
#include <osg/ComputeBoundsVisitor>
#include <osg/MatrixTransform>
#include <QDebug>
FirstPersonManipulator::FirstPersonManipulator()
{
    m_vPosition=osg::Vec3f(-16.4533 ,-20 ,5);
    m_vRotation = osg::Vec3(1.5252, -0.00135134, 0.0277073);
    m_vStep = 5.0f;
    m_vRotateStep = 0.05f;
    m_bLeftDown = false;
    m_pickObj =nullptr;
}

FirstPersonManipulator::~FirstPersonManipulator()
{
}

osg::Matrixd FirstPersonManipulator::getMatrix() const
{
    osg::Matrixd mat;
    mat.makeTranslate(m_vPosition);
    return osg::Matrixd::rotate(m_vRotation[0], osg::X_AXIS, m_vRotation[1], osg::Y_AXIS, m_vRotation[2], osg::Z_AXIS)*mat;
}

osg::Matrixd FirstPersonManipulator::getInverseMatrix() const
{
    osg::Matrixd mat;
    mat.makeTranslate(m_vPosition);
    return osg::Matrixd::inverse(osg::Matrixd::rotate(m_vRotation[0], osg::X_AXIS, m_vRotation[1], osg::Y_AXIS, m_vRotation[2], osg::Z_AXIS)*mat);
}

bool FirstPersonManipulator::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
{
    osgViewer::Viewer* view = dynamic_cast<osgViewer::Viewer*>(&us);
    if(osgGA::GUIEventAdapter::KEYDOWN==ea.getEventType()){
        keyboraKeyDown(view,ea);
    }else if(osgGA::GUIEventAdapter::SCROLL==ea.getEventType()){
        mouseWheelScroll(view,ea);
    }else if(osgGA::GUIEventAdapter::PUSH==ea.getEventType()&&ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON){
        mouseLeftPush(view,ea);
    }else if(osgGA::GUIEventAdapter::RELEASE==ea.getEventType()&&ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON){
        mouseLeftRelease(view,ea);
    }else if(osgGA::GUIEventAdapter::DRAG==ea.getEventType()&&m_bLeftDown){
        mouseLeftDrage(view,ea);
    }else if(osgGA::GUIEventAdapter::DRAG==ea.getEventType()&&m_bRightDown){
        mouseRightDrage(view,ea);
    }else if(osgGA::GUIEventAdapter::PUSH==ea.getEventType()&&ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON){
        mouseRightPush(view,ea);
    }else if(osgGA::GUIEventAdapter::RELEASE==ea.getEventType()&&ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON){
        mouseRightRelease(view,ea);
    }
    return false;
}
void FirstPersonManipulator::mouseRightDrage(osgViewer::Viewer* view,const osgGA::GUIEventAdapter &ea){
    _isDragging=true;
}
void FirstPersonManipulator::mouseRightRelease(osgViewer::Viewer* ,const osgGA::GUIEventAdapter &){
    m_bLeftDown = false;
    _isDragging=false;
    m_bRightDown=false;
}
void FirstPersonManipulator::mouseRightPush(osgViewer::Viewer* view,const osgGA::GUIEventAdapter &ea){
    rayLinePick(view,ea.getX(),ea.getY());
    m_bLeftDown = false;
    _isDragging=false;
    m_bRightDown=true;
    m_iLeftX = ea.getX();
    m_iLeftY = ea.getY();
}
void FirstPersonManipulator::mouseLeftDrage(osgViewer::Viewer* view,const osgGA::GUIEventAdapter &ea){
    if(m_pickObj){
        _isDragging=true;
        //计算当前摄像机与pick到的模型之间的距离是多少
        osg::Vec3 offset = m_pickPoint - m_vPosition;
        int dist = offset.length();
        //计算当前的鼠标屏幕点映射到三维中的值
        osg::Vec3 mouseWorldPos = screenToWorld(view,ea.getX(),ea.getY());
        //计算当前鼠标三维点与摄像机的方向
        osg::Vec3 rayDir = (mouseWorldPos - m_vPosition);
        rayDir.normalize();
        //最后计算物体拖拽时最终的世界位置
        osg::Vec3 curPos=m_vPosition+rayDir*dist;
        m_pickObj->setMatrix(osg::Matrix::translate(curPos));
    }
    if(_isDragging) return;

    int delX = ea.getX() - m_iLeftX;
    int delY = ea.getY() - m_iLeftY;
    m_vRotation[2] -= osg::DegreesToRadians(m_vRotateStep * delX);
    m_vRotation[0] += osg::DegreesToRadians(m_vRotateStep * delY);
    if (m_vRotation[0] <= 1.3f)
    {
        m_vRotation[0] = 1.3f;
    }
    if (m_vRotation[0] >= 1.5f)
    {
        m_vRotation[0] = 1.5f;
    }
    m_iLeftX = ea.getX();
    m_iLeftY = ea.getY();
}
void FirstPersonManipulator::mouseLeftRelease(osgViewer::Viewer*,const osgGA::GUIEventAdapter &){
    m_bLeftDown = false;
    _isDragging=false;
}
void FirstPersonManipulator::mouseLeftPush(osgViewer::Viewer* view,const osgGA::GUIEventAdapter &ea){
    rayLinePick(view,ea.getX(),ea.getY());
    m_iLeftX = ea.getX();
    m_iLeftY = ea.getY();
    m_bLeftDown = true;
    _isDragging=false;
}
void FirstPersonManipulator::mouseWheelScroll(osgViewer::Viewer* view,const osgGA::GUIEventAdapter &ea){
    osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();
    if (sm == osgGA::GUIEventAdapter::SCROLL_DOWN){
        osg::Vec3 vNormalize = Screen2World(view, ea.getX(), ea.getY());
        vNormalize *= m_vStep;
        ChangePosition(vNormalize);
    }
    else
    {
        osg::Vec3 vNormalize = Screen2World(view, ea.getX(), ea.getY());
        vNormalize *= -m_vStep;
        ChangePosition(vNormalize);
    }
}
void FirstPersonManipulator::keyboraKeyDown(osgViewer::Viewer* view,const osgGA::GUIEventAdapter &ea){
    if (ea.getKey() == 'w' || ea.getKey() == 'W')
    {
        osg::Vec3 vNormalize = -Screen2World(view, ea.getX(), ea.getY());
        vNormalize *= m_vStep;
        m_vPosition._v[0]+=vNormalize.x();
        m_vPosition._v[1]+=vNormalize.y();
    }
    else if (ea.getKey() == 's' || ea.getKey() == 'S')
    {
        osg::Vec3 vNormalize = Screen2World(view, ea.getX(), ea.getY());
        vNormalize *= m_vStep;
        m_vPosition._v[0]+=vNormalize.x();
        m_vPosition._v[1]+=vNormalize.y();
    }
    else if (ea.getKey() == 'a' || ea.getKey() == 'A')
    {
        ChangePosition(osg::Vec3(-m_vStep * sinf(osg::PI_2 + m_vRotation._v[2]), m_vStep * cosf(osg::PI_2 + m_vRotation._v[2]), 0));
    }
    else if (ea.getKey() == 'd' || ea.getKey() == 'D')
    {
        ChangePosition(osg::Vec3(m_vStep * sinf(osg::PI_2 + m_vRotation._v[2]), -m_vStep * cosf(osg::PI_2 + m_vRotation._v[2]), 0));
    }
    else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_KP_Add || ea.getKey() == osgGA::GUIEventAdapter::KEY_Equals)
    {
        SetStep(2);
    }
    else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_KP_Subtract || ea.getKey() == osgGA::GUIEventAdapter::KEY_Minus)
    {
        SetStep(-2);
    }
    else if (ea.getKey() == 'q' || ea.getKey() == 'Q')
    {
        ChangePosition(osg::Vec3(0.0, 0.0, 1));
    }
    else if (ea.getKey() == 'e' || ea.getKey() == 'E')
    {
        ChangePosition(osg::Vec3(0.0, 0.0, -1.0));
    }
}
bool FirstPersonManipulator::ChangePosition(osg::Vec3 delta)
{
    m_vPosition += delta;
    return true;
}

void FirstPersonManipulator::SetStep(int delta_step)
{
    m_vStep += delta_step;
    if (m_vStep <= 1)
        m_vStep = 1;
}

void FirstPersonManipulator::getTransformation(osg::Vec3 &eye, osg::Vec3 &rota)
{
    eye=m_vPosition;
    rota=m_vRotation;
}

osg::Vec3 FirstPersonManipulator::screenToWorld(osgViewer::Viewer *viewer, double dx, double dy)
{
    osg::Camera *camera = viewer->getCamera();
    osg::Matrix viewMat = camera->getViewMatrix(); //获取当前视图矩阵
    osg::Matrix projMat = camera->getProjectionMatrix();//获取投影矩阵
    osg::Matrix windMat = camera->getViewport()->computeWindowMatrix();//获取窗口矩阵
    osg::Matrix MVPW = viewMat * projMat *windMat;  //视图-》投影-》窗口变换
    osg::Matrix inverseMVPW = osg::Matrix::inverse(MVPW);
    osg::Vec3 mouseWorld = osg::Vec3(dx, dy, 0) * inverseMVPW;
    return mouseWorld;
}

void FirstPersonManipulator::rayLinePick(osgViewer::Viewer* view, float x, float y)
{
    //创建一个线段交集检测函数
    osgUtil::LineSegmentIntersector::Intersections intersections;
    if (view->computeIntersections(x, y, intersections)){
        osgUtil::LineSegmentIntersector::Intersection intersection = *intersections.begin();
        //得到选择的物体
        for(auto iter = intersections.begin(); iter != intersections.end(); ++iter){
            osg::NodePath nodePath = iter->nodePath;
            m_pickPoint = iter->getWorldIntersectPoint();
            for(int i=0; i<nodePath.size(); ++i)
            {
                m_pickObj = dynamic_cast<osg::MatrixTransform*>(nodePath[i]);
                if(nullptr!=m_pickObj) return;
            }
        }
    }
}

osg::Vec3 FirstPersonManipulator::Screen2World(osgViewer::Viewer* view, float x, float y)
{
    osg::ref_ptr<osg::Camera> camera = view->getCamera();


    osg::Vec3 vWindow(x, y, 0);
    osg::Matrix mVPW = camera->getViewMatrix() * camera->getProjectionMatrix() * camera->getViewport()->computeWindowMatrix();
    osg::Matrix invertMVPW;
    invertMVPW.invert(mVPW);
    osg::Vec3 vLocal = vWindow * invertMVPW;


    osg::Vec3 vView = osg::Vec3(0, 0, 0);
    osg::Matrix mView = camera->getViewMatrix();
    osg::Matrix inverseMView;
    inverseMView.invert(camera->getViewMatrix());
    osg::Vec3 vViewWorld = vView * inverseMView;

    osg::Vec3 vMouse = vViewWorld - vLocal;
    vMouse.normalize();
    return vMouse;
}
