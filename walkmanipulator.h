/*

The Walk Manipulator allows the following movements with the listed
      Key combinations:
        \param UpArrow          向前移动
        \param DownArrow        向后移动
        \param LeftArrow        向左旋转
        \param RightArrow       向右旋转
        \param SpaceBar         回到默认位置
        \param Shift/UpArrow    向上移动
        \param Shift/DownArrow  向下移动
        \param Shift/LeftArrow  向左移动
        \param Shift/RightArrow 向右移动
        \param Ctrl/UpArrow     仰上旋转
        \param Ctrl/DownArrow   俯下旋转
        \param Ctrl/LeftArrow   向左滚转
        \param Ctrl/RightArrow  向右滚转
    */
#ifndef OSGGA_WALK_MANIPULATOR_DEF
#define OSGGA_WALK_MANIPULATOR_DEF 1


#include <iostream>

#include <osgGA/CameraManipulator>
#include <osg/Node>
#include <osg/Matrix>
#include<osgGA/GUIEventAdapter>
#include<osgGA/GUIActionAdapter>

class WalkManipulator : public osgGA::CameraManipulator
{

    public:
        /** Default constructor */
        WalkManipulator();

        /** return className */
        virtual const char* className() const;

        /** set the position of the matrix manipulator using a 4x4 Matrix.*/
        virtual void setByMatrix( const osg::Matrixd &matrix ) { setByInverseMatrix(osg::Matrixd::inverse(matrix)); }

        /** get the position of the manipulator as 4x4 Matrix.*/
        virtual osg::Matrixd getMatrix() const;

        /** set the position of the matrix manipulator using a 4x4 Matrix.*/
        virtual void setByInverseMatrix(const osg::Matrixd& matrix);

        /** get the position of the manipulator as a inverse matrix of the manipulator, typically used as a model view matrix.*/
        virtual osg::Matrixd getInverseMatrix() const;

        /** Set the  subgraph this manipulator is driving the eye through. root of subgraph */
        virtual void setNode(osg::Node* node);

        /** Get the root node of the subgraph this manipulator is driving the eye through (const)*/
        virtual const osg::Node* getNode() const;

        /** Get the root node of the subgraph this manipulator is driving the eye through */
        virtual osg::Node* getNode();

        /** Sets the viewpoint matrix to the home position */
        virtual void home(const  osgGA::GUIEventAdapter &ea,  osgGA::GUIActionAdapter &aa);
        virtual void computeHomePosition();
        virtual void home(double);
    //    virtual void init(const  osgGA::GUIEventAdapter &ea,  osgGA::GUIActionAdapter &aa);

        /** Handles incoming osgGA events */
        virtual bool handle(const  osgGA::GUIEventAdapter &ea,  osgGA::GUIActionAdapter &aa);

        /** Reports Usage parameters to the application */
        void getUsage(osg::ApplicationUsage& usage) const;

        void setForwardSpeed( double in_fs ) { _forwardSpeed = in_fs; }
        double getForwardSpeed() const { return _forwardSpeed; }

        void setSideSpeed( double in_ss ) { _sideSpeed = in_ss; }
        double getSideSpeed() const { return _sideSpeed; }

        void setUpSpeed( double in_us ) { _upSpeed = in_us; }
        double getUpSpeed() const { return _upSpeed; }

        void setPitchAngle( double in_pa ) { _pitchAngle = in_pa; }
        double getPitchAngle() const { return _pitchAngle; }

        void setRollAngle( double in_ra ) { _rollAngle = in_ra; }
        double getRollAngle() const { return _rollAngle; }

        void setYawAngle( double in_ya ) { _yawAngle = in_ya; }
        double getYawAngle() const { return _yawAngle; }

        void setModelScale( double in_ms ) { _modelScale = in_ms; }
        double getModelScale() const { return _modelScale; }

        void adjustFactor(bool accel);
        osg::Vec3d getEye() const { return _eye; }

    protected:

        virtual ~WalkManipulator() {}

        osg::ref_ptr<osg::Node> _node;

        double _modelScale;				// 模型的大小

        // 视点的位置与姿态
        osg::Vec3d _eye;					// 当前视点的位置

        // 当前视点的局部坐标系
        osg::Vec3d _sideVector;
        osg::Vec3d _frontVector;
        osg::Vec3d _upVector;

    //osg::Vec3d target;

        double    _speedEpsilon;		// 速度的精度值,小于该值速度为零
        double    _forwardSpeed;		// 前后移动的速度
        double    _sideSpeed;			// 左右移动的速度
        double    _upSpeed;				// 上下移动的速度
        double    _speedFactor;			// 移动速度调节因子

        double    _angleEpsilon;		// 旋转角度的精度值,小于该值旋转角度为零
        double    _pitchAngle;			// 俯仰旋转的角度
        double    _rollAngle;			// 滚转旋转的角度
        double    _yawAngle;			// 偏航旋转的角度
        double    _angleFactor;			// 旋转角度调节因子

        bool _shift;					// 是否按下Shift键
        bool _ctrl;						// 是否按下Ctrl键
    //	bool _collision;                // 是否支持碰撞检测

        void _stop();					// 使移动速度和旋转角度均为零
        void _keyDown( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
        void _keyUp( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

    //	void computePosition(const osg::Vec3& eye, const osg::Vec3& lv, const osg::Vec3& up);

        void _moveFront(bool positive);
        void _moveSide(bool positive);
        void _moveUp(bool positive);

        void _rotatePitch(bool positive);
        void _rotateRoll(bool positive);
        void _rotateYaw(bool positive);

    //	void _collisionDetect(osg::Vec3& delta);
};

#endif
