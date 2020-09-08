
#include "ChessBoard.h"
#include "ui_ChessBoard.h"

ChessBoard::ChessBoard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChessBoard)
{

    init();

    //计时器部分
    m_timer = new QTimer;  //初始化定时器
    m_timeRecord  = new QTime(0, 0, 0); //初始化时间
    m_bIsStart = false;  //初始为还未计时
    connect(m_timer,SIGNAL(timeout()),this,SLOT(updateTime()));

   // m_pAbout = new AboutAuthor();

    this->setWindowIcon(QIcon(":/images/qaz.ico"));
    ui->setupUi(this);
}

ChessBoard::~ChessBoard()
{
    delete ui;
}

void ChessBoard::init()
{
    for(int i = 0; i<32; i++)
    {
        m_ChessPieces[i].init(i);
    }

    m_nSelectID = -1;
    m_nCheckedID = -1;
    m_bIsTcpServer = true;
    m_bIsRed = true;
    m_bIsOver = false;
}

bool ChessBoard::isDead(int id)
{
    if(id == -1)return true;
    return m_ChessPieces[id].m_bDead;
}

int ChessBoard::getStoneId(int row, int col)
{
    for(int i=0; i<32; ++i)
    {
        if(m_ChessPieces[i].m_nRow == row && m_ChessPieces[i].m_nCol == col && !isDead(i))
            return i;
    }
    return -1;
}



//象棋的棋盘的坐标转换成界面坐标
QPoint ChessBoard::center(int row, int col)
{
    QPoint rePoint;
    //这里注意坐标的转换
    rePoint.ry() = row*m_nD+m_nOffSet + 45;
    rePoint.rx() = col*m_nD+m_nOffSet + 45;

    return rePoint;
}

//重载:坐标转换
QPoint ChessBoard::center(int id)
{
    return center(m_ChessPieces[id].m_nRow , m_ChessPieces[id].m_nCol);
}

void ChessBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int side = qMin(int((ui->centralwidget->width() - ui->verticalWidget->width()) / 0.9), ui->label->height());//0.9->0.1
    painter.scale(side / 960.0, side / 960.0);

        m_nOffSet =100;  //距离界面的边距 60
        m_nD = 90; //间距为50px
        m_nR = m_nD/2;  //棋子半径为d/2

        //*******************绘画棋盘*******************
        //绘画10条横线
        for(int i = 0; i <= 9; i++)
        {
            painter.drawLine(QPoint(m_nOffSet, m_nOffSet+i*m_nD), QPoint(m_nOffSet+8*m_nD, m_nOffSet+i*m_nD));
        }


        //绘画9条竖线
        for(int i = 0; i <= 8; i++)
        {
            if(i==0 || i==8)
            {
                painter.drawLine(QPoint(m_nOffSet+i*m_nD, m_nOffSet), QPoint(m_nOffSet+i*m_nD, m_nOffSet+9*m_nD));
            }
            else
            {
                painter.drawLine(QPoint(m_nOffSet+i*m_nD, m_nOffSet), QPoint(m_nOffSet+i*m_nD, m_nOffSet+4*m_nD));
                painter.drawLine(QPoint(m_nOffSet+i*m_nD, m_nOffSet+5*m_nD), QPoint(m_nOffSet+i*m_nD, m_nOffSet+9*m_nD));
            }
        }

        //绘画4条斜线
        painter.drawLine(QPoint(m_nOffSet+3*m_nD, m_nOffSet), QPoint(m_nOffSet+5*m_nD, m_nOffSet+2*m_nD));
        painter.drawLine(QPoint(m_nOffSet+3*m_nD, m_nOffSet+2*m_nD), QPoint(m_nOffSet+5*m_nD, m_nOffSet));
        painter.drawLine(QPoint(m_nOffSet+3*m_nD, m_nOffSet+7*m_nD), QPoint(m_nOffSet+5*m_nD, m_nOffSet+9*m_nD));
        painter.drawLine(QPoint(m_nOffSet+3*m_nD, m_nOffSet+9*m_nD), QPoint(m_nOffSet+5*m_nD, m_nOffSet+7*m_nD));

        QRect rect1(m_nOffSet+m_nD,   m_nOffSet+4*m_nD, m_nD, m_nD);
        QRect rect2(m_nOffSet+2*m_nD, m_nOffSet+4*m_nD, m_nD, m_nD);
        QRect rect3(m_nOffSet+5*m_nD, m_nOffSet+4*m_nD, m_nD, m_nD);
        QRect rect4(m_nOffSet+6*m_nD, m_nOffSet+4*m_nD, m_nD, m_nD);
        painter.setFont(QFont("隶书", m_nR, 800));
        painter.drawText(rect1, "楚", QTextOption(Qt::AlignCenter));
        painter.drawText(rect2, "河", QTextOption(Qt::AlignCenter));
        painter.drawText(rect3, "汉", QTextOption(Qt::AlignCenter));
        painter.drawText(rect4, "界", QTextOption(Qt::AlignCenter));


        //*******************绘画棋子*******************
        for(int i = 0; i < 32; i++)
        {
            drawChessPieces(painter, i);
        }

        //drawChessPieces(painter, 0);

}

void ChessBoard::drawChessPieces(QPainter &painter, int id)   //绘画单个具体的棋子
{
    if(m_ChessPieces[id].m_bDead)
        return;

    QPoint temp = center(id);
    QRect rect(temp.x()-m_nR, temp.y()-m_nR, m_nD, m_nD);
    if(m_ChessPieces[id].m_bon)
    {
        if(m_nSelectID == id)
            painter.setBrush(QBrush(QColor(64,64,196, 90)));
        else
            painter.setBrush(QBrush(QColor(64,64,196, 10)));

    }
    else
    {
        painter.setBrush(QBrush(QColor(169,169,169, 50)));
    }
    painter.setPen(QColor(0, 0, 0));
    painter.drawEllipse(center(id), m_nR, m_nR);  //绘画圆形
   //
    painter.setFont(QFont("华文行楷", m_nR, 700));

    if(m_ChessPieces[id].m_bon)
    {
        if(id < 16)
        {
            painter.setPen(QColor(0, 0, 0));
        }
        else
        {
            painter.setPen(QColor(255, 0, 0));
        }
        painter.drawText(rect, m_ChessPieces[id].getnName(), QTextOption(Qt::AlignCenter));  //绘画圆形里面的汉字
    }



}

//

QPoint ChessBoard::getRealPoint(QPoint pt){  // 使mouseMoveEvent取得的坐标同Painter的坐标一致
    int side = qMin(int((ui->centralwidget->width() - ui->verticalWidget->width()) / 0.9), ui->label->height());
    QPoint ret;

    ret.setX(pt.x() / double(side) * 960.0);
    ret.setY(pt.y() / double(side) * 960.0);

    return ret;
}


//鼠标点击事件 图形转坐标
void ChessBoard::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() != Qt::LeftButton || ev->type() != QEvent::Type::MouseButtonPress)
    {
        //只响应鼠标左键的单击操作 防止游戏结束重复弹框
        return;
    }

    QPoint pt = ev->pos();
    pt = getRealPoint(pt);
    //将pt转化成棋盘的像行列值
    //判断这个行列值上面有没有棋子
    int row, col;

    //点击棋盘外面就不做处理
    if(!isChecked(pt, row, col))
        return;

    if(m_bIsOver)
    {
        QMessageBox message(QMessageBox::Information, "提示", "本局已结束，请重新开始.");
       // message.setIconPixmap(QPixmap(":/images/Is.JPG"));
        message.setFont(QFont("华文行楷",16,QFont::Bold));
        message.exec();
        return;
    }

    //判断是哪一个棋子被选中，根据ID（这里的局部i）来记录下来
    int i;
    m_nCheckedID = -1; //不一定会被击杀，暂时不考虑等级

    for(i = 0; i <= 31; i++)
    {
        if(m_ChessPieces[i].m_nRow == row && m_ChessPieces[i].m_nCol == col && m_ChessPieces[i].m_bDead == false)
            break;
    }



    if(0<=i && i<32)
        m_nCheckedID = i;  //选中的棋子的ID


    clickPieces(m_nCheckedID, row, col);  //点击

    update();
  //  whoWin();
}
void ChessBoard::clickPieces(int checkedID, int& row, int& col)
{

    m_nCheckedID = checkedID;

    if(m_nSelectID == -1)//选中棋子
    {
       // whoPlay(m_nCheckedID);

        if(m_nCheckedID != -1)
        {
            if(m_bIsRed == m_ChessPieces[m_nCheckedID].m_bRed)
            {
                m_nSelectID = m_nCheckedID;
            }
        }
    }
    else//走棋子
    {
        if(canMove(m_nSelectID, m_nCheckedID, row, col ))  //重写canmove
        {
            //m_nSelectID为第一次点击选中的棋子，
            //m_nCheckedID为第二次点击||被杀的棋子ID，准备选中棋子下子的地方
            m_ChessPieces[m_nSelectID].m_nRow = row;
            m_ChessPieces[m_nSelectID].m_nCol = col;
            if(m_nCheckedID != -1)
                m_ChessPieces[m_nCheckedID].m_bDead = true;

            m_nSelectID = -1;
            m_bIsRed = !m_bIsRed;
        }

    }
}
bool ChessBoard::isChecked(QPoint pt, int &row, int &col) //修改成上半区
{
    for(row = 0; row <= 3; row++)
    {
        for(col = 0; col <= 7; col++)
        {
            QPoint temp = center(row, col);
            int x = temp.x()-pt.x();
            int y = temp.y()-pt.y();

            if(x*x+y*y < m_nR*m_nR)
                return true;
        }
    }
    return false;
}


bool ChessBoard::canMove(int moveId, int killId, int row, int col)
{
    //1.确定是选择其它棋子还是走棋
    //2.是否需要使用到canMoveXXX()来做限制
    //3.罗列出所有情况，和需要的得到的结果值 ==>  然后进行中间的逻辑层判断※不要受到别人的代码框架的束缚※
        if(m_ChessPieces[moveId].m_bRed == m_ChessPieces[killId].m_bRed)  //选择其它棋子，返回false
        {
            m_nSelectID = killId;
            return false;
        }
        else  //选择其走棋，返回true
        {

            return true;
        }




}

//总的移动规则，选中准备下的棋子，被杀的棋子， 准备移动到的目的行列值
//能否移动
//    //1.确定是选择其它棋子还是走棋
//    //2.是否需要使用到canMoveXXX()来做限制
//    //3.罗列出所有情况，和需要的得到的结果值 ==>  然后进行中间的逻辑层判断※不要受到别人的代码框架的束缚※



//}
//棋子具体移动

//判断移动
//bool ChessBoard::canMoveBING(int moveId, int killId, int row, int col)


//关于按钮

//刷新时间
void ChessBoard::updateTime()
{
    *m_timeRecord = m_timeRecord->addSecs(1);
    ui->lcdNumber->display(m_timeRecord->toString("hh:mm:ss"));

    if(m_bIsStart == false)
    {
        ui->pushButton_start->setText("开始");
    }
    else if(m_bIsStart == true)
    {
        ui->pushButton_start->setText("暂停");
    }
}


void ChessBoard::on_pushButton_start_clicked()
{
    if(!m_bIsStart) //尚未开始 开始计时
        {
            m_timer->start(1000);
            ui->pushButton_start->setText("暂停");
        }
        else //已经开始，暂停
        {
            m_timer->stop();
            ui->pushButton_start->setText("继续");
        }
        m_bIsStart = !m_bIsStart;
}

void ChessBoard::on_pushButton_reset_clicked()
{
    m_timer->stop();    //计时器停止
    m_timeRecord->setHMS(0,0,0); //时间设为0
    ui->lcdNumber->display(m_timeRecord->toString("hh:mm:ss")); //显示00:00:00
    m_bIsStart = false;
    ui->pushButton_start->setText("开始");
    ui->pushButton_start->setEnabled(true);
}



void ChessBoard::on_pushButton_restart_clicked()
{
    init();
    on_pushButton_reset_clicked();
    update();
}



