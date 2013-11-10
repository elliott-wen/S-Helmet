#include "viewermanager.h"
#include <QDebug>

ViewerManager::ViewerManager()
{
    this->_viewer=0;
    this->freshRate=0;

}
osgQt::GraphicsWindowQt* ViewerManager::createGraphicsWindow( int x, int y, int w, int h, const std::string& name, bool windowDecoration )
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->windowName = name;
    traits->windowDecoration = windowDecoration;
    traits->x = x;
    traits->y = y;
    traits->width = w;
    traits->height = h;
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();
    return new osgQt::GraphicsWindowQt(traits.get());
}
QWidget* ViewerManager::createOpenGLViewWidget()
{
      osgQt::GraphicsWindowQt* gw=createGraphicsWindow(0,0,INTITAL_WIDTH,INTITAL_HEIGHT);
      this->_viewer = new osgViewer::Viewer;
      this->_viewer->setThreadingModel(this->_viewer->suggestBestThreadingModel());
      this->_viewer->setKeyEventSetsDone(0);
      osg::Camera* camera = this->_viewer->getCamera();
      camera->setGraphicsContext( gw );
      const osg::GraphicsContext::Traits* traits = gw->getTraits();
      camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
      camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
      camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );
      this->_viewer->addEventHandler( new osgViewer::StatsHandler );
      this->_viewer->setCameraManipulator( new WalkManipulator );
      //this->_viewer->setCameraManipulator(new osgGA::TrackballManipulator);
      return gw->getGLWidget();
}
void ViewerManager::setMaxFrameRate(int frameRate)
{
    if(this->_viewer!=0)
    {
        this->_viewer->setRunMaxFrameRate(frameRate);
    }
}


void ViewerManager::startViewerRender()
{
    this->connect(&this->refreshTimer,SIGNAL(timeout()),this,SLOT(updateViewer()));
    this->refreshTimer.start(UPDATEINTERVAL);
}
void ViewerManager::stopViewerRender()
{
    this->refreshTimer.stop();
}
osg::Geode* ViewerManager::drawGround()
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
void ViewerManager::setSceneDataSource(const QString &data)
{
    this->dataSource=data;
}

int ViewerManager::buildScene()
{
    osg::ref_ptr<osg::Group> scene=new osg::Group;
    osg::ref_ptr<osg::PositionAttitudeTransform> matrixTransform=new osg::PositionAttitudeTransform;
    osg::ref_ptr<osg::Node> buildModel=osgDB::readNodeFile(this->dataSource.toLatin1().data());
    if(!buildModel.valid())
    {
        return BUILDSCENEFAILED;
    }
    matrixTransform->addChild(buildModel.get());
    matrixTransform->setPosition(osg::Vec3d(0,0,0));
    scene->addChild(matrixTransform);
    osg::ref_ptr<osg::Geode> ground=drawGround();
    scene->addChild(ground.get());
    this->_viewer->setSceneData(scene.get());
    return BUILDSCENESUCCESSFUL;

}
void ViewerManager::updateViewer()
{

        if(!_viewer->done())
        {
            _viewer->frame();
        }
}
