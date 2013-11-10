#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include <QProgressDialog>
#include "viewermanager.h"
#include "aboutdialog.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *ui;
    ViewerManager viewerManager;
    QWidget *viewerWidget;
    bool _sessionStarted;
    QProgressDialog *loadingProgressDialog;

private slots:
    void openSession();
    void closeSession();
    void shutdownApp();
    void aboutDialog();
    void afterBuild(int result);

};

#endif // MAINWINDOW_H
