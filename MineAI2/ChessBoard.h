#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <QMainWindow>
#include <QFrame>        //QFrame而不是用QWidget???
#include "ChessPieces.h"
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QtGlobal>
#include <QMainWindow>
#include <QCheckBox>
#include <QLabel>
#include<QWidget>

namespace Ui {         //具体作用???
class ChessBoard;      //???
}

class ChessBoard : public QMainWindow
{
    Q_OBJECT

public:
    QLabel pic;
    explicit ChessBoard(QWidget *parent = 0);
    ~ChessBoard();
    bool isDead(int id);
    int deadj ;
    int getStoneId(int row, int col); //炮吃子功能辅助函数   判断两个点是否在一个直线上面,且返回直线之间的棋子个数
    int  getStoneCountAtLine(int row1, int col1, int row2, int col2);
    virtual void isWin();  //谁胜谁负
    virtual void copymap(int * arr);
    bool isChecked(QPoint pt, int& row, int& col);   //是否选中该枚棋子。pt为输入参数; row， col为输出参数
    QPoint getRealPoint(QPoint pt);  // 使mouseMoveEvent取得的坐标同Painter的坐标一致
    virtual void mousePressEvent(QMouseEvent *);    //鼠标点击事件
    virtual void clickPieces(int checkedID, int& row, int& col);
    QPoint center(int row, int col);         //象棋的棋盘的坐标转换成界面坐标
    QPoint center(int id);
    virtual void paintEvent(QPaintEvent *);      //绘画棋盘
    void drawChessPieces(QPainter& painter, int id);  //绘画单个具体的棋子
    virtual void movedown(int moveId, int row,int col); //移入下半区
    bool move(int moveId, int row, int col);
    void init();
     bool aicanMove(int moveId, int killId);
    int tmpid;  //新ai用，用作记录移动过程位置的id
    ChessPieces m_ChessPieces[32];  ///所有棋子
    int red_num = 16;
    int black_num = 16;
    int round_num = 0;
    int m_nR;          //棋子半径
    int m_nOffSet;     //距离界面的边距/
    int m_nD;          //间距为50px/
    int m_nSelectID;   //选中棋子[-1:选棋子 || 非-1:走棋子]/
    int m_nCheckedID;  //将要被击杀的棋子ID/
    bool m_bIsRed;     //是否是红棋/
    bool m_bIsTcpServer; //联机
   // bool m_bIsOver; //是否已经游戏结束/
    bool aired;
    bool aiblack;

     virtual bool canMove(int moveId, int killId, int row, int col);
     bool canMoveJIANG(int moveId, int killId, int row, int col);
     bool canMovePAO(int moveId, int killId, int row, int col);
     bool canMoveBING(int moveId, int killId, int row, int col);
     bool canMoveNORMAL(int moveId, int killId, int row, int col);
     void aimove(int camp);
     int lookingmoveid (int  camp);//返回寻找最大的己方id，炮不考虑，无返回-1
     int lookingsecondid (int  camp,int moveid); //寻找第二大的
      int lookingthirdid (int  camp,int moveid,int sec); //寻找第二大的
     int lookingkillid(int  camp,int moveid); //寻找能吃的最大id，找不到翻棋子
     int len(int x,int y, int killid);//返回长度
     void moveai(int moveid, int killid);//上下左右的顺序移动，若新len小于原来，就移动，若四个都不行，翻棋子
     void movenolen(int camp,int moveid, int killid);//不考虑len的上下左右的顺序移动
     bool ismove(int id,int x,int y);//判断能不能移动
     void AiTurnOn();//AI随机翻棋，若不能，就让最大的上下左右动
private slots:
    void updateTime();
    void updateai();
    void on_pushButton_start_clicked();
    void on_pushButton_reset_clicked();
    void onrStateChanged(int state);
    void onbStateChanged(int state);
    void on_pushButton_restart_clicked();

private:
    Ui::ChessBoard *ui;
    QTimer * m_timer;  //定时器 每秒更新时间
    QTimer *timer;
    QTime * m_timeRecord;  //记录时间
    bool m_bIsStart;        //记录是否已经开始计时
};

#endif // CHESSBOARD_H
