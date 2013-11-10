#ifndef BUILDSCENETHREAD_H
#define BUILDSCENETHREAD_H

#include <QThread>
#include "viewermanager.h"

class BuildSceneThread : public QThread
{
    Q_OBJECT
public:
    explicit BuildSceneThread(ViewerManager *viewer,QObject *parent = 0);
protected:
    void run();
private:
    ViewerManager *_viewerManager;
signals:
    void buildComplete(int result);
public slots:
    
};

#endif // BUILDSCENETHREAD_H
