#ifndef VIEWERTHREAD_H
#define VIEWERTHREAD_H

#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>
#include <QTimer>
#define UNABLE_OPEN_SOURCE -111
#define OPEN_SOURCE_SUCCESS 0
class ViewerThread:public QObject
{
    Q_OBJECT
public:
    ViewerThread(osgViewer::Viewer *viewer);
    void startViewer();
    void stopViewer();
    int loadSceneData();
    ~ViewerThread();
    int cancel();
    bool runFlag;
    void setDataSource(const std::string &fileName);

public slots:
    void drawViewer();
private:
    osg::ref_ptr<osgViewer::Viewer> _viewer;
    std::string dataSource;
    osg::Geode* drawGround();
    QTimer timer;
    bool hasLoadData;
    
};

#endif // VIEWERTHREAD_H
