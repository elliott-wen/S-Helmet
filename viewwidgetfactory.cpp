#include "viewwidgetfactory.h"
#include <QDebug>
ViewWidgetFactory::ViewWidgetFactory()
{
    setKeyEventSetsDone(0);
    setThreadingModel(this->suggestBestThreadingModel());

}

QWidget* ViewWidgetFactory::createOpenGLViewWidget( const std::string &fileName )
  {
    osgQt::GraphicsWindowQt* gw=createGraphicsWindow(0,0,INTITAL_WIDTH,INTITAL_HEIGHT);
      osg::Node *scene=osgDB::readNodeFile(fileName);
      osgViewer::View* view = new osgViewer::View;
      view->addEventHandler(new osgViewer::StatsHandler);
      addView( view );
      osg::Camera* camera = view->getCamera();
      camera->setGraphicsContext( gw );
      const osg::GraphicsContext::Traits* traits = gw->getTraits();
      camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
      camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
      camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );
      view->setSceneData(scene);
      view->addEventHandler( new osgViewer::StatsHandler );
      view->setCameraManipulator( new osgGA::TrackballManipulator );

      return gw->getGLWidget();
  }

  osgQt::GraphicsWindowQt* ViewWidgetFactory::createGraphicsWindow( int x, int y, int w, int h, const std::string& name, bool windowDecoration )
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
  void ViewWidgetFactory::updateViewerFrame()
  {
      frame();
  }
  void ViewWidgetFactory::startViewerRender()
  {
        connect(&_timer,SIGNAL(timeout()),this,SLOT(updateViewerFrame()));
        this->_timer.start(1000/VIEWER_REFRESH_RATE);
  }
  void ViewWidgetFactory::stopViewerRender()
  {
        disconnect(&_timer,SIGNAL(timeout()),this,SLOT(updateViewerFrame()));
        this->_timer.stop();

  }
