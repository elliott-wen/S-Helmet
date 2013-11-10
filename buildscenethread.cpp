#include "buildscenethread.h"

BuildSceneThread::BuildSceneThread(ViewerManager *viewer, QObject *parent) :
    QThread(parent)
{
    this->_viewerManager=viewer;
}

void BuildSceneThread::run()
{
    int result=this->_viewerManager->buildScene();
    emit((buildComplete(result)));
}
