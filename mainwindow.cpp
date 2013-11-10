#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

#include "buildscenethread.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    viewerWidget=0;
    this->_sessionStarted=false;
    connect(ui->actionOpenSession,SIGNAL(triggered()),this,SLOT(openSession()));
    connect(ui->actionCloseSession,SIGNAL(triggered()),this,SLOT(closeSession()));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(aboutDialog()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::openSession()
{
    QString dataSource=QFileDialog::getOpenFileName(this,"Scene","","All Files (*)");
    if(dataSource.isEmpty()) return;
    ui->welcomelabel->hide();
    ui->actionCloseSession->setEnabled(true);
    ui->actionOpenSession->setEnabled(false);
    viewerManager.setSceneDataSource(dataSource);
    viewerWidget=this->viewerManager.createOpenGLViewWidget();
    ui->horizontalLayout->addWidget(viewerWidget);
    loadingProgressDialog=new QProgressDialog(this);
    loadingProgressDialog->setLabelText(tr("Loading"));
    loadingProgressDialog->setRange(0,100);
    loadingProgressDialog->setModal(true);
    loadingProgressDialog->setCancelButton(0);
    loadingProgressDialog->setValue(60);
    loadingProgressDialog->show();
    BuildSceneThread *thread=new BuildSceneThread(&viewerManager,this);
    connect(thread,SIGNAL(buildComplete(int)),this,SLOT(afterBuild(int)));
    thread->start();
}

void MainWindow::afterBuild(int result)
{
    if(result==BUILDSCENESUCCESSFUL)
    {
        this->viewerManager.startViewerRender();
        this->_sessionStarted=true;
    }
    else
    {
        QMessageBox::warning(this,tr("Error"),tr("Error When trying to load data"));
        this->shutdownApp();
        qApp->exit(-1);
    }
    loadingProgressDialog->hide();
}

void MainWindow::closeSession()
{
    this->viewerManager.stopViewerRender();
    ui->horizontalLayout->removeWidget(viewerWidget);
    this->viewerWidget->deleteLater();
    this->viewerWidget=0;
    ui->welcomelabel->show();
    ui->actionCloseSession->setEnabled(false);
    ui->actionOpenSession->setEnabled(true);
    this->_sessionStarted=false;
}
void MainWindow::shutdownApp()
{
    if(this->_sessionStarted)
    {
        this->closeSession();
    }
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, "Exit Anyway?",
            QString("Warning: Would you like to exit anyway?"),
            QMessageBox::Yes | QMessageBox::No);
    if (button == QMessageBox::No)
    {
        event->ignore();  //忽略退出信号，程序继续运行
    }
    else if (button == QMessageBox::Yes)
    {
        shutdownApp();
        event->accept();  //接受退出信号，程序退出
    }
}
void MainWindow::aboutDialog()
{
    AboutDialog *dialog=new AboutDialog(this);
    dialog->show();
}
