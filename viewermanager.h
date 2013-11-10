#ifndef VIEWERMANAGER_H
#define VIEWERMANAGER_H
#include <QWidget>
#include <QTimer>
#include <QApplication>
#include <QGridLayout>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osgQt/GraphicsWindowQt>
#include <osg/PositionAttitudeTransform>
#include <QObject>
#include <string>
#include "walkmanipulator.h"
#include "config.h"
#include <osg/ShapeDrawable>
#include <osg/TexGen>
using namespace std;

class ViewerManager:public QObject
{
    Q_OBJECT
public:
    ViewerManager();
    QWidget* createOpenGLViewWidget();
    void startViewerRender();
    void stopViewerRender();
    void setMaxFrameRate(int maxRate);
    void setSceneDataSource(const QString& data);
    void prepareScene();
    int buildScene();
private:
    QString dataSource;
    osg::Geode* drawGround();
    QTimer refreshTimer;
    osgViewer::Viewer *_viewer;
    int freshRate;
    osgQt::GraphicsWindowQt* createGraphicsWindow( int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false );
signals:
    
public slots:
    void updateViewer();
    
};

#endif // VIEWERMANAGER_H
