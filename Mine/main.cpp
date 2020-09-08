#include "mainwindow.h"
#include <QApplication>
#include "ChessBoard.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChessBoard w;
    w.setWindowTitle("暗棋-by 张华祥 NJUST 2020");
    w.setWindowIcon(QIcon(":/source/img/logo.png"));
//    w.setFixedSize(760, 620);
    w.show();

    return a.exec();
}
