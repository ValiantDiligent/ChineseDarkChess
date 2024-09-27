#include "mainwindow.h"
#include <QApplication>
#include "ChessBoard.h"
#include "networkgame.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMessageBox::StandardButton isnet;
    isnet = QMessageBox::question(NULL,"networkgame?","是否联网对战？");
    if(isnet == QMessageBox::Yes)
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::question(NULL,"server or client","是否作为服务器启动？");
        bool bServer = false;
        if(ret == QMessageBox::Yes)
        {
            bServer = true;
        }
        NetworkGame w1(bServer);
        w1.setWindowTitle("红方【网络对战】");
        qDebug()<<"你选择的是："<<bServer;
        w1.setWindowIcon(QIcon(":/source/img/qaz.ico"));
        w1.show();
        NetworkGame w2(!bServer);
        w2.setWindowTitle("黑方【网络对战】");
        w2.show();
        return a.exec();
    }
    else
    {
        ChessBoard w;
        w.setWindowTitle("暗棋-by 张华祥 NJUST 2020");
        w.setWindowIcon(QIcon(":/source/img/qaz.ico"));
        w.show();
        return a.exec();
    }



}
