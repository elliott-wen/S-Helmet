#include "viewerthread.h"

ViewerThread::ViewerThread(osgViewer::Viewer *viewer)
{
    this->_viewer=viewer;
    runFlag=false;
    hasLoadData=false;
}

void ViewerThread::drawViewer()
{
    if(!hasLoadData)
    {
        if(this->loadSceneData()!=OPEN_SOURCE_SUCCESS)
        {
            return;
        }
    }
        if(!_viewer->done())
        {
            _viewer->frame();
        }

}
ViewerThread::~ViewerThread()
{
    timer.stop();
}

void ViewerThread::stopViewer()
{
    timer.stop();
    _viewer->setDone(true);

}
void ViewerThread::startViewer()
{

    timer.connect(&timer,SIGNAL(timeout()),this,SLOT(drawViewer()));
    timer.start(30);
}
int ViewerThread::cancel()
{
    stopViewer();
    return 0;
}
int ViewerThread::loadSceneData()
{
    osg::ref_ptr<osg::Group> scene=new osg::Group;
    osg::ref_ptr<osg::PositionAttitudeTransform> matrixTransform=new osg::PositionAttitudeTransform;
    osg::ref_ptr<osg::Node> buildModel=osgDB::readNodeFile(this->dataSource);
    if(!buildModel.valid())
    {
        return UNABLE_OPEN_SOURCE;
    }
    matrixTransform->addChild(buildModel.get());
    matrixTransform->setPosition(osg::Vec3d(0,0,0));
    scene->addChild(matrixTransform);
    osg::ref_ptr<osg::Geode> ground=drawGround();
    scene->addChild(ground.get());
    this->_viewer->setSceneData(scene.get());
    hasLoadData=true;
    return OPEN_SOURCE_SUCCESS;
}
void ViewerThread::setDataSource(const std::string &fileName)
{
    this->dataSource=fileName;
}
osg::Geode* ViewerThread::drawGround()
{
    osg::ref_ptr<osg::Geode> geode =new osg::Geode;
    osg::ref_ptr<osg::Geometry> geom=new osg::Geometry;
    osg::ref_ptr<osg::Vec3Array> v=new osg::Vec3Array;
    GLint count=0;
    for(float i=-100;i<=100;i++)
    {
        count+=4;
        v->push_back(osg::Vec3(-100.f,i,0.f));
        v->push_back(osg::Vec3(100.f,i,0.f));
        v->push_back(osg::Vec3(i,-100.f,0.f));
        v->push_back(osg::Vec3(i,100.f,0.f));
   }
   geom->setVertexArray(v.get());
   osg::ref_ptr<osg::Vec4Array> c=new osg::Vec4Array;
   c->push_back(osg::Vec4(1.0f,1.0f,1.0f,1.f));
   geom->setColorArray(c.get());
   geom->setColorBinding(osg::Geometry::BIND_OVERALL);
   geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,count));
   geode->addDrawable(geom.get());
   geode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
   return geode.release();
}
