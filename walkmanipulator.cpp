#include "walkmanipulator.h"
#include <osgUtil/IntersectVisitor>

#ifndef M_PI
# define M_PI       3.14159265358979323846  /* pi */
#endif

WalkManipulator::WalkManipulator():
    _shift(false),_ctrl(false)
{
    _speedEpsilon=0.02;
    _speedFactor=0.2;
    _angleEpsilon=0.0001;
    _angleFactor=0.1;
    _upVector.set(0,0,1);
    _forwardSpeed=0.2;
    _sideSpeed=0.2;
    _upSpeed=0.2;
    _pitchAngle=0.1;
    _rollAngle=0.1;
    _yawAngle=0.1;
}
void WalkManipulator::_stop()
{
    _forwardSpeed=0.0;
    _sideSpeed=0.0;
    _upSpeed=0.0;
    _pitchAngle=0.0;
    _rollAngle=0.0;
    _yawAngle=0.0;
}
const char* WalkManipulator::className() const
{
    return "WalkManipulator";
}
void WalkManipulator::getUsage(osg::ApplicationUsage &usage) const
{
    usage.addKeyboardMouseBinding("Walk Manipulator: <UpArrow>",                  "Move forward.");
    usage.addKeyboardMouseBinding("Walk Manipulator: <DownArrow>",                "Move back.");
    usage.addKeyboardMouseBinding("Walk Manipulator: <LeftArrow>",                "Rolate Left.");
    usage.addKeyboardMouseBinding("Walk Manipulator: <RightArrow>",               "Rolate Right.");
    usage.addKeyboardMouseBinding("Walk Manipulator: <Shift/UpArrow>",            "Move up.");
    usage.addKeyboardMouseBinding("Walk Manipulator: <Shift/DownArrow>",          "Move down.");
    usage.addKeyboardMouseBinding("Walk Manipulator: <Shift/LeftArrow>",          "Move left.");
    usage.addKeyboardMouseBinding("Walk Manipulator: <Shift/RightArrow>",         "Move right.");
    usage.addKeyboardMouseBinding("Walk Manipulator: <Ctrl/UpArrow>",             "Pitch up.");
    usage.addKeyboardMouseBinding("Walk Manipulator: <Ctrl/DownArrow>",           "Pitch down.");
    usage.addKeyboardMouseBinding("Walk Manipulator: <Ctrl/LeftArrow>",           "Roll left.");
    usage.addKeyboardMouseBinding("Walk Manipulator: <Ctrl/RightArrow>",          "Roll Right.");
}
void WalkManipulator::setNode(osg::Node *node )
{
    _node=node;
    if (_node.get())
    {
        const osg::BoundingSphere& boundingSphere=_node->getBound();
        _modelScale = boundingSphere._radius;
    }

}
const osg::Node* WalkManipulator::getNode() const
{
    return _node.get();
}

osg::Node* WalkManipulator::getNode()
{
    return _node.get();
}

void WalkManipulator::setByInverseMatrix(const osg::Matrixd &matrix)
{
    osg::Matrixd mat=matrix;
    mat=osg::Matrixd::inverse(mat);
    _eye=mat.getTrans();
    osg::CoordinateFrame cf=getCoordinateFrame(_eye);
    _frontVector =getFrontVector(cf);
    _sideVector  =getSideVector(cf);
    _upVector    =getUpVector(cf);
}
osg::Matrixd WalkManipulator::getMatrix() const
{
    osg::Matrix mat(_sideVector[0],_upVector[0],-_frontVector[0],     0.0,
                    _sideVector[1],_upVector[1],-_frontVector[1],     0.0,
                    _sideVector[2],_upVector[2],-_frontVector[2],     0.0,
                               0.0,         0.0,            0.0,     1.0);

    return mat*osg::Matrix::translate(_eye);
}
osg::Matrixd WalkManipulator::getInverseMatrix() const
{
    osg::Matrix mat(_sideVector[0],_upVector[0],-_frontVector[0],     0.0,
                    _sideVector[1],_upVector[1],-_frontVector[1],     0.0,
                    _sideVector[2],_upVector[2],-_frontVector[2],     0.0,
                               0.0,         0.0,            0.0,     1.0);

    return osg::Matrix::translate(-_eye)*mat;
}
void WalkManipulator::computeHomePosition()
{

    if( !_node.valid() )
        return;

    osg::BoundingSphere bs = _node->getBound();

    /*
       * Find the ground - Assumption: The ground is the hit of an intersection
       * from a line segment extending from above to below the database at its
       * horizontal center, that intersects the database closest to zero. */
    osgUtil::IntersectVisitor iv;
    iv.setTraversalMask(_intersectTraversalMask);

    osg::CoordinateFrame cf( getCoordinateFrame(bs.center()) ); // not sure what position to use here
    osg::Vec3d upVec( getUpVector(cf) );
    osg::ref_ptr<osg::LineSegment> seg = new osg::LineSegment;
    osg::Vec3 A = bs.center() + (upVec*(bs.radius()*2));
    osg::Vec3 B = bs.center() + (-upVec*(bs.radius()*2));

    if( (B-A).length() == 0.0)
    {
        return;
    }

    /*
    seg->set( bs.center() + (upVec*(bs.radius()*2)),
              bs.center() + (-upVec*(bs.radius()*2)) );
              */
    seg->set( A, B );

    iv.addLineSegment( seg.get() );
    _node->accept(iv);

    // start with it high
    double ground = bs.radius() * 3;

    if (iv.hits())
    {
        osgUtil::IntersectVisitor::HitList& hitList = iv.getHitList(seg.get());
        osg::Vec3d ip = hitList.front().getWorldIntersectPoint();
        double d = ip.length();
        if( d < ground )
            ground = d;
    }
    else
    {
        //osg::notify(osg::WARN)<<"UFOManipulator : I can't find the ground!"<<std::endl;
        ground = 0.0;
    }
    osg::Vec3 p(bs.center() + upVec*( ground + 2.0*1.25 ) );
    setHomePosition( p, p + getFrontVector(cf), upVec );
}
void WalkManipulator::home(double )
{
    if (getAutoComputeHomePosition()) computeHomePosition();

    _eye=_homeEye;
    _frontVector=_homeCenter-_homeEye;
    _frontVector.normalize();
    _upVector=_homeUp;
    _sideVector=_frontVector^_upVector;
}
void WalkManipulator::home(const  osgGA::GUIEventAdapter &ea,osgGA::GUIActionAdapter &aa)
{
    if (getAutoComputeHomePosition())
        computeHomePosition();
    _eye=_homeEye;
    _frontVector=_homeCenter-_homeEye;
    _frontVector.normalize();
    _upVector=_homeUp;
    _sideVector=_frontVector^_upVector;

}
bool WalkManipulator::handle(const  osgGA::GUIEventAdapter &ea,osgGA::GUIActionAdapter &aa)
{

    if (ea.getHandled()) return false;

    switch(ea.getEventType())
    {
        case(osgGA::GUIEventAdapter::KEYUP):
            _keyUp( ea, aa );
            return false;
            break;

        case(osgGA::GUIEventAdapter::KEYDOWN):
            _keyDown(ea, aa);
            return false;
            break;
        default:
            return false;
    }
    return false;
}
void WalkManipulator::_keyUp(const  osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
    switch(ea.getKey())
    {
        case osgGA::GUIEventAdapter::KEY_Control_L:
        case osgGA::GUIEventAdapter::KEY_Control_R:
            _ctrl = false;
            break;

        case osgGA::GUIEventAdapter::KEY_Shift_L :
        case osgGA::GUIEventAdapter::KEY_Shift_R :
            _shift = false;
            break;
    }
}
void WalkManipulator::_keyDown(const  osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
    switch(ea.getKey())
    {
        case osgGA::GUIEventAdapter::KEY_Control_L:
        case osgGA::GUIEventAdapter::KEY_Control_R:
            _ctrl = true;
            break;

        case osgGA::GUIEventAdapter::KEY_Shift_L :
        case osgGA::GUIEventAdapter::KEY_Shift_R :
            _shift = true;
            break;
        case osgGA::GUIEventAdapter::KEY_Up :
            if(_ctrl)
            {
                _rotatePitch(true);
                aa.requestRedraw();
                aa.requestContinuousUpdate(false);
                break;
            }
            else
            {
                if(_shift)
                {
                    _moveUp(true);
                    aa.requestRedraw();
                    aa.requestContinuousUpdate(false);
                    break;
                }
                else
                {
                    _moveFront(true);
                    aa.requestRedraw();
                    aa.requestContinuousUpdate(false);
                    break;
                }
            }
        case osgGA::GUIEventAdapter::KEY_Down:
            if(_ctrl)
            {
                _rotatePitch(false);
                aa.requestRedraw();
                aa.requestContinuousUpdate(false);
                break;
            }
            else
            {
                if(_shift)
                {
                    _moveUp(false);
                    aa.requestRedraw();
                    aa.requestContinuousUpdate(false);
                    break;
                }
                else
                {
                    _moveFront(false);
                    aa.requestRedraw();
                    aa.requestContinuousUpdate(false);
                    break;
                }
            }
        case osgGA::GUIEventAdapter::KEY_Left:
            if(_ctrl)
            {
                _rotateRoll(true);
                aa.requestRedraw();
                aa.requestContinuousUpdate(false);
                break;
            }
            else
            {
                if(_shift)
                {
                    _moveSide(false);
                    aa.requestRedraw();
                    aa.requestContinuousUpdate(false);
                    break;
                }
                else
                {
                    _rotateYaw(true);
                    aa.requestRedraw();
                    aa.requestContinuousUpdate(false);
                    break;
                }
            }
        case osgGA::GUIEventAdapter::KEY_Right:
            if(_ctrl)
            {
                _rotateRoll(false);
                aa.requestRedraw();
                aa.requestContinuousUpdate(false);
                break;
            }
            else
            {
                if(_shift)
                {
                    _moveSide(true);
                    aa.requestRedraw();
                    aa.requestContinuousUpdate(false);
                    break;
                }
                else
                {
                    _rotateYaw(false);
                    aa.requestRedraw();
                    aa.requestContinuousUpdate(false);
                    break;
                }
            }

    }
}
void WalkManipulator::_moveFront(bool positive)
{
    if(positive)
    {
        _eye+=_frontVector*_forwardSpeed;
    }
    else
    {
        _eye-=_frontVector*_forwardSpeed;
    }
}
void WalkManipulator::_moveSide(bool positive)
{
    if(positive)
    {
        _eye+=_sideVector*_forwardSpeed;
    }
    else
    {
        _eye-=_sideVector*_forwardSpeed;
    }
}
void WalkManipulator::_moveUp(bool positive)
{
    if(positive)
    {
        _eye+=_upVector*_forwardSpeed;
    }
    else
    {
        _eye-=_upVector*_forwardSpeed;
    }
}
void WalkManipulator::_rotateYaw(bool positive)
{

    if(positive)
    {
        _frontVector=_frontVector*osg::Matrixd::rotate(_yawAngle,_upVector);
        _sideVector=_sideVector*osg::Matrixd::rotate(_yawAngle,_upVector);

    }
    else
    {
        _frontVector=_frontVector*osg::Matrixd::rotate(-_yawAngle,_upVector);
        _sideVector=_sideVector*osg::Matrixd::rotate(-_yawAngle,_upVector);
    }
}
void WalkManipulator::_rotatePitch(bool positive)
{
    if(positive)
    {
        _frontVector=_frontVector*osg::Matrixd::rotate(_pitchAngle,_sideVector);
        _upVector=_upVector*osg::Matrixd::rotate(_pitchAngle,_sideVector);
    }
    else
    {
        _frontVector=_frontVector*osg::Matrixd::rotate(-_pitchAngle,_sideVector);
        _upVector=_upVector*osg::Matrixd::rotate(-_pitchAngle,_sideVector);
    }
}
void WalkManipulator::_rotateRoll(bool positive)
{
    if(positive)
    {
        _sideVector=_sideVector*osg::Matrixd::rotate(_rollAngle,_frontVector);
        _upVector=_upVector*osg::Matrixd::rotate(_rollAngle,_frontVector);
    }
    else
    {
        _sideVector=_sideVector*osg::Matrixd::rotate(-_rollAngle,_frontVector);
        _upVector=_upVector*osg::Matrixd::rotate(-_rollAngle,_frontVector);
    }
}
