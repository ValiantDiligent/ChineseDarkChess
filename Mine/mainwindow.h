#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMessageBox>
#include <QIcon>
#include <QLabel>
#include <QMovie>
#include <QSize>
#include <QKeyEvent>
#include <QAction>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QLineEdit>
#include <QTime>
#include <QFile>
#include <QInputDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
