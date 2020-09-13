
#include "ChessBoard.h"
#include "ui_ChessBoard.h"
static int deadj = 0;
int red_num = 16;
int black_num = 16;
int round_num = 0;
ChessBoard::ChessBoard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChessBoard)
{
    init();
    //计时器部分
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateai()));
    timer->start(1000);

    aired = 0;
    aiblack = 0;
    m_timer = new QTimer;  //初始化定时器
    m_timeRecord  = new QTime(0, 0, 0); //初始化时间
    m_bIsStart = false;  //初始为还未计时
    connect(m_timer,SIGNAL(timeout()),this,SLOT(updateTime()));
    //connect(m_timer,SIGNAL(timeout()),this,SLOT(updateai()));
    this->setWindowIcon(QIcon(":/images/qaz.ico"));
    ui->setupUi(this);
    //AI判定部分
    connect(ui->rCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onrStateChanged(int)));
    connect(ui->bCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onbStateChanged(int)));
}

ChessBoard::~ChessBoard()
{
    delete ui;}
void ChessBoard::init()//初始化棋盘
{

    //随机分布
    m_ChessPieces[33].getmap();//让棋盘随机分布
    for(int i = 0; i<32; i++)
    {
        m_ChessPieces[i].init(i);
    }
    deadj = 0;
    m_nSelectID = -1;
    m_nCheckedID = -1;
    m_bIsTcpServer = true;
    m_bIsRed = true;
   // m_bIsOver = false;
     red_num = 16;
     black_num = 16;
     round_num = 0;
     aimoveId = -1; //被选中棋子
     airow = -1;  //目标行
    aicol = -1;  //目标列

}
bool ChessBoard::isDead(int id)
{
    if(id == -1)return true;
    return m_ChessPieces[id].m_bDead;
}
//炮辅助函数
int ChessBoard::getStoneId(int row, int col)//从坐标返回id
{
    for(int i=0; i<32; ++i)
    {
        if(m_ChessPieces[i].m_nRow == row && m_ChessPieces[i].m_nCol == col && !isDead(i))
            return i;
    }
    return -1;
}
int ChessBoard::getStoneCountAtLine(int row1, int col1, int row2, int col2)
{
    int ret = 0;
    if(row1 != row2 && col1 != col2)
        return -1;
    if(row1 == row2 && col1 == col2)
        return -1;

    if(row1 == row2)
    {
        int min  = col1 < col2 ? col1 : col2;
        int max = col1 < col2 ? col2 : col1;
        for(int col = min+1; col<max; ++col)
        {
            if(getStoneId(row1, col) != -1) ++ret;
        }
    }
    else
    {
        int min = row1 < row2 ? row1 : row2;
        int max = row1 < row2 ? row2 : row1;
        for(int row = min+1; row<max; ++row)
        {
            if(getStoneId(row, col1) != -1) ++ret;
        }
    }

    return ret;

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
QPoint ChessBoard::center(int id)
{
    return center(m_ChessPieces[id].m_nRow , m_ChessPieces[id].m_nCol);
}

void ChessBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int side = qMin(int((ui->centralwidget->width() - ui->verticalWidget->width()) / 0.9), ui->label->height());//0.9->0.1
    painter.scale(side / 960.0, side / 960.0);

        m_nOffSet =100;  //距离界面的边距
        m_nD = 90; //间距为
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
    if(m_ChessPieces[id].m_bDead  &&  m_ChessPieces[id].m_nRow < 3 )//
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

    //点击棋盘外面就取消原有选择
    if(!isChecked(pt, row, col))
    {
        m_nSelectID = -1; //颜色还没刷新
      //  update();
        return;
    }
    int i;
    m_nCheckedID = -1; //不一定会被击杀，暂时不考虑等级

    for(i = 0; i <= 31; i++)
    {
        if(m_ChessPieces[i].m_nRow == row && m_ChessPieces[i].m_nCol == col && m_ChessPieces[i].m_bDead == false)
            break;
    }
    if(0<=i && i<32)
        m_nCheckedID = i;  //目标棋子的ID
    clickPieces(m_nCheckedID, row, col);  //点击
    update();
}
void ChessBoard::clickPieces(int checkedID, int& row, int& col)//点到棋子了
{

    m_nCheckedID = checkedID;

    if(m_nSelectID == -1)//如果原先没有选中棋子
    {

        if(m_nCheckedID != -1) //若目标棋子存在，即没点空白
        {
            if(m_ChessPieces[m_nCheckedID].m_bon && m_bIsRed == m_ChessPieces[m_nCheckedID].m_bRed) //轮到谁下，选对颜色了
            {
                m_nSelectID = m_nCheckedID;
            }
            if(!m_ChessPieces[m_nCheckedID].m_bon) //反面变正面，进入下一回合
            {
                m_ChessPieces[m_nCheckedID].m_bon = 1;
                m_nSelectID = -1;
                m_bIsRed = !m_bIsRed;
                round_num++;
            }
        }
    }
    else//能翻棋就翻棋 走棋子
    {
        if(canMove(m_nSelectID, m_nCheckedID, row, col ))  //重写canmove判断能不能走
        {
            //round_num++;
            m_ChessPieces[m_nSelectID].m_nRow = row;
            m_ChessPieces[m_nSelectID].m_nCol = col;
            if(m_nCheckedID != -1)
            {
                m_ChessPieces[m_nCheckedID].m_bDead = true;
                movedown(m_nCheckedID, row, col);
            }
            m_nSelectID = -1;
            m_bIsRed = !m_bIsRed;
            update();
        }
        isWin();

    }
}
bool ChessBoard::isChecked(QPoint pt, int &row, int &col) //检查是否点到棋子
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

bool ChessBoard::canMove(int moveId, int killId, int row, int col) //能否移动
{
        if(m_ChessPieces[moveId].m_bRed == m_ChessPieces[killId].m_bRed && m_ChessPieces[killId].m_bon)  //选择其它棋子，返回false
        {
            if(killId == -1)
            {
                if(move(moveId,row,col))
                    round_num++;
                return  move(moveId,row,col);

            }
            m_nSelectID = killId;
            return false;
        }
        else if(!m_ChessPieces[killId].m_bon)
        {

            if(killId == -1)
            {
                if(move(moveId,row,col))
                    round_num++;
                return  move(moveId,row,col);

            }
             return false;
        }
        else  //选择其走棋，返回true
        {
            //常规移动

            if(killId == -1)
            {
                if(move(moveId,row,col))
                    round_num++;
                return  move(moveId,row,col);

            }
            else
            {
                switch (m_ChessPieces[moveId].m_emType)
                {
                case ChessPieces::JIANG:
                {
                    if(canMoveJIANG(moveId, killId, row, col))
                    {
                        round_num = 0;
                        if(m_bIsRed)
                        {
                            black_num--;
                        }
                        else
                        {
                            red_num--;
                        }
                    }
                    return canMoveJIANG(moveId, killId, row, col);
                }

                case ChessPieces::PAO:
                {
                                    if(canMovePAO(moveId, killId, row, col))
                                    {
                                        round_num = 0;
                                        if(m_bIsRed)
                                        {
                                            black_num--;
                                        }
                                        else
                                        {
                                            red_num--;
                                        }
                                    }
                                    return canMovePAO(moveId, killId, row, col);
                                }

                case ChessPieces::BING:
                {
                    if(canMoveBING(moveId, killId, row, col))
                    {
                        round_num = 0;
                        if(m_bIsRed)
                        {
                            black_num--;
                        }
                        else
                        {
                            red_num--;
                        }
                    }
                    return canMoveBING(moveId, killId, row, col);
                }

                default:
                {
                    if(canMoveNORMAL(moveId, killId, row, col))
                    {
                        round_num = 0;
                        if(m_bIsRed)
                        {
                            black_num--;
                        }
                        else
                        {
                            red_num--;
                        }
                    }
                     return canMoveNORMAL(moveId, killId, row, col);
                }

                }
                return true;
            }



        }
        isWin();
}
bool ChessBoard::canMoveJIANG(int moveId, int killId, int row, int col)
{
    int Nret = m_ChessPieces[moveId].m_level -  m_ChessPieces[killId].m_level;
    int dr = m_ChessPieces[moveId].m_nRow - row;
    int dc = m_ChessPieces[moveId].m_nCol - col;
    int d = abs(dr)*10 + abs(dc);
    if((d == 1 || d == 10) && Nret != 6)
        return true;

    return false;
}
bool ChessBoard::canMoveBING( int moveId, int killId, int row, int col)
{
    int Nret = m_ChessPieces[moveId].m_level -  m_ChessPieces[killId].m_level;
    int dr = m_ChessPieces[moveId].m_nRow - row;
    int dc = m_ChessPieces[moveId].m_nCol - col;
    int d = abs(dr)*10 + abs(dc);
    if((d == 1 || d == 10 ) && (Nret == 0 || Nret == -6))
        return true;

    return false;
}
bool ChessBoard::canMovePAO ( int moveId, int killId, int row, int col)
{
    int num = getStoneCountAtLine(row, col, m_ChessPieces[moveId].m_nRow, m_ChessPieces[moveId].m_nCol);
    int Nret = m_ChessPieces[moveId].m_level -  m_ChessPieces[killId].m_level;
    int dr = m_ChessPieces[moveId].m_nRow - row;
    int dc = m_ChessPieces[moveId].m_nCol - col;
    int d = abs(dr)*10 + abs(dc);
   // if(d == 1 || d == 10 && Nret != -1 )
    if(num == 1  && Nret != -1 )
        return true;

    return false;
}
bool ChessBoard::canMoveNORMAL ( int moveId, int killId, int row, int col)
{
    int Nret = m_ChessPieces[moveId].m_level -  m_ChessPieces[killId].m_level;
    int dr = m_ChessPieces[moveId].m_nRow - row;
    int dc = m_ChessPieces[moveId].m_nCol - col;
    int d = abs(dr)*10 + abs(dc);
    if((d == 1 || d == 10) && Nret >= 0 )
        return true;

    return false;
}

bool ChessBoard::move(int moveId, int row, int col) //移动 空格
{
    int dr = m_ChessPieces[moveId].m_nRow - row;
    int dc = m_ChessPieces[moveId].m_nCol - col;
    int d = abs(dr)*10 + abs(dc);
    if(d == 1 || d == 10 )
        return true;

    return false;
    //

}
void ChessBoard::movedown(int moveId,  int row, int col)//死亡棋子移动到下半区
{
    m_ChessPieces[moveId].m_bon  = 1 ;
    m_ChessPieces[moveId].m_nRow = 5  + deadj/8;
    m_ChessPieces[moveId].m_nCol = deadj++ % 8;
    return ;

}
void ChessBoard::isWin()
{
    if(m_bIsStart)
    {
        m_timer->stop();
        m_bIsStart = false;
    }
    ui->pushButton_start->setEnabled(false);
    if(red_num == 0)
    {
        QMessageBox message(QMessageBox::Information, "提示", "本局结束，黑方胜利.");
       // message.setIconPixmap(QPixmap(":/images/Is.JPG"));
        message.setFont(QFont("华文行楷",16,QFont::Bold));
        message.exec();
    }
    if(black_num == 0)
    {
        QMessageBox message(QMessageBox::Information, "提示", "本局结束，红方胜利.");
        //message.setIconPixmap(QPixmap(":/images/Is.JPG"));
        message.setFont(QFont("华文行楷",16,QFont::Bold));
        message.exec();
    }
    if(round_num == 25)
    {
        QMessageBox message(QMessageBox::Information, "提示", "二十五回合未吃子，和棋.");
        message.setFont(QFont("华文行楷",16,QFont::Bold));
        message.exec();
    }
}
//刷新时间
void ChessBoard::updateai()
{
    if(aired && m_bIsRed)
    {
        aimove(1);
    }
    else if(aiblack && !m_bIsRed)
    {
        aimove(0);
    }
}
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
            m_timer->start(2000);
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
//自动走棋
void ChessBoard::AiTurnOn()//AI随机翻棋
{
    int i;
    int r = 0;
    i =rand()%32;
    while(1)
    {
        if(!m_ChessPieces[i].m_bon) //反面变正面，进入下一回合
        {
            m_ChessPieces[i].m_bon = 1;
            m_nSelectID = -1;
            m_bIsRed = !m_bIsRed;
            round_num++;
             update();
            return;
        }
        else if(r >= 32)
        {
            //return
        }
        else
        {
            i++;
            r++;
            i = i%32;
        }
    }

}
void ChessBoard::aimove(int isred)//AI移动吃子
{

 if(Harvest(isred) > enemyHarvest(isred))
 {
     m_nSelectID = aimoveId;
     int i;
     m_nCheckedID = -1;
     for(i = 0; i <= 31; i++)
     {
         if(m_ChessPieces[i].m_nRow == tairow && m_ChessPieces[i].m_nCol == taicol && m_ChessPieces[i].m_bDead == false)
             break;
     }
     if(0<=i && i<32)
         m_nCheckedID = i;  //目标棋子的ID
     clickPieces(m_nCheckedID, tairow, taicol);
      update();
 }
 else
 {
     AiTurnOn();
 }
}
int ChessBoard::enemyHarvest(int isred)//吃子辅助函数 返回敌人一回合内最优吃子得分
{

    int ret = -1;
    for(int i =0; i<32; i++)
    {
        if(m_ChessPieces[i].m_bon&& !m_ChessPieces[i].m_bDead && (m_ChessPieces[i].m_bRed != isred ) && (aicheck(i,!isred) >ret))
        {
            ret = aicheck(i,!m_ChessPieces[i].m_bRed);
        }
    }
    return ret;



}
int ChessBoard::Harvest(int isred)//吃子辅助函数 返回自己两回合最优吃子得分
{
    int ret = -1;
    for(int i =0; i<32; i++)
    {
        if(m_ChessPieces[i].m_bon&& !m_ChessPieces[i].m_bDead && (m_ChessPieces[i].m_bRed==isred ) && (aicheck(i,isred) >ret))
        {
            ret = aicheck(i,m_ChessPieces[i].m_bRed);
            aimoveId = i;
            tairow = airow;
            taicol = aicol;
        }
    }
    return ret;
}
int ChessBoard::aicheck(int id,int camp) //Harvest辅助函数/ 返回该店最优得分
{
    int ret = -1;
    int aipieceid = id;
    int tmp;
    aipiece = camp;
    int x =m_ChessPieces[id].m_nRow ,y= m_ChessPieces[id].m_nCol;
    if(m_bIsRed == camp)//判断两步
    {
        if(rtlevel(x + 1,y )> ret )//如果返回等级大于原有收益
        {

            ret = rtlevel(x + 1,y);
            airow =m_ChessPieces[tmpid].m_nRow;
            aicol =m_ChessPieces[tmpid].m_nCol;
            //
            if(rtlevel(x + 2,y )> ret )//如果返回等级大于原有收益
            {

                ret = rtlevel(x + 2,y);
                //airow =m_ChessPieces[tmpid].m_nRow ;
                //aicol =m_ChessPieces[tmpid].m_nCol;
            }

            if(rtlevel(x +1 ,y -1 )> ret )//如果返回等级大于原有收益
            {

                ret = rtlevel(x+1,y -1);
               // airow =m_ChessPieces[tmpid].m_nRow ;
                //aicol =m_ChessPieces[tmpid].m_nCol;
            }
            if(rtlevel(x +1 ,y+ 1 )> ret )//如果返回等级大于原有收益
            {

                ret = rtlevel(x+1,y + 1);
                //airow =m_ChessPieces[tmpid].m_nRow ;
               // aicol =m_ChessPieces[tmpid].m_nCol;
            }
            //
        }
        if(rtlevel(x - 1,y )> ret )//如果返回等级大于原有收益
        {

            ret = rtlevel(x -1,y);
            airow =m_ChessPieces[tmpid].m_nRow;
            aicol =m_ChessPieces[tmpid].m_nCol;
            //

            if(rtlevel(x - 2,y )> ret )//如果返回等级大于原有收益
            {

                ret = rtlevel(x -2,y);
               // airow =m_ChessPieces[tmpid].m_nRow;
                //aicol =m_ChessPieces[tmpid].m_nCol;
            }
            if(rtlevel(x-1 ,y -1 )> ret )//如果返回等级大于原有收益
            {

                ret = rtlevel(x-1,y -1);
              //  airow =m_ChessPieces[tmpid].m_nRow;
               // aicol =m_ChessPieces[tmpid].m_nCol;
            }
            if(rtlevel(x-1 ,y+ 1 )> ret )//如果返回等级大于原有收益
            {

                ret = rtlevel(x-1,y + 1);
                //airow =m_ChessPieces[tmpid].m_nRow;
               // aicol =m_ChessPieces[tmpid].m_nCol;
            }
            //
        }
        if(rtlevel(x ,y -1 )> ret )//如果返回等级大于原有收益
        {

            ret = rtlevel(x,y -1);
            airow =m_ChessPieces[tmpid].m_nRow;
            aicol =m_ChessPieces[tmpid].m_nCol;
            //
            if(rtlevel(x + 1,y-1 )> ret )//如果返回等级大于原有收益
            {

                ret = rtlevel(x + 1,y-1);
              //  airow =m_ChessPieces[tmpid].m_nRow;
                //aicol =m_ChessPieces[tmpid].m_nCol;
            }
            if(rtlevel(x - 1,y -1)> ret )//如果返回等级大于原有收益
            {

                ret = rtlevel(x -1,y-1);
                //airow =m_ChessPieces[tmpid].m_nRow;
              //  aicol =m_ChessPieces[tmpid].m_nCol;
            }
            if(rtlevel(x ,y -1-1 )> ret )//如果返回等级大于原有收益
            {

                ret = rtlevel(x,y -1-1);
               // airow =m_ChessPieces[tmpid].m_nRow;
               // aicol =m_ChessPieces[tmpid].m_nCol;
            }
            //
        }
        if(rtlevel(x ,y+ 1 )> ret )//如果返回等级大于原有收益
        {

            ret = rtlevel(x,y + 1);
            airow =m_ChessPieces[tmpid].m_nRow;
            aicol =m_ChessPieces[tmpid].m_nCol;
            //
            if(rtlevel(x + 1,y+ 1 )> ret )//如果返回等级大于原有收益
            {

                ret = rtlevel(x + 1,y+ 1);
               // airow =m_ChessPieces[tmpid].m_nRow;
               // aicol =m_ChessPieces[tmpid].m_nCol;
            }
            if(rtlevel(x - 1,y+ 1 )> ret )//如果返回等级大于原有收益
            {

                ret = rtlevel(x -1,y+ 1);
              //  airow =m_ChessPieces[tmpid].m_nRow;
              //  aicol =m_ChessPieces[tmpid].m_nCol;
            }
            if(rtlevel(x ,y + 1+ 1 )> ret )//如果返回等级大于原有收益
            {

                ret = rtlevel(x,y + 1+ 1);
               // airow =m_ChessPieces[tmpid].m_nRow;
               // aicol =m_ChessPieces[tmpid].m_nCol;
            }

            //
        }

    }
    else//判断一步
    {
        if(rtlevel(x + 1,y )> ret )//如果返回等级大于原有收益
        {

            ret = rtlevel(x + 1,y);
            airow =m_ChessPieces[tmpid].m_nRow;
            aicol =m_ChessPieces[tmpid].m_nCol;
        }
        if(rtlevel(x - 1,y )> ret )//如果返回等级大于原有收益
        {

            ret = rtlevel(x -1,y);
            airow =m_ChessPieces[tmpid].m_nRow;
            aicol =m_ChessPieces[tmpid].m_nCol;
        }
        if(rtlevel(x ,y -1 )> ret )//如果返回等级大于原有收益
        {

            ret = rtlevel(x,y -1);
            airow =m_ChessPieces[tmpid].m_nRow;
            aicol =m_ChessPieces[tmpid].m_nCol;
        }
        if(rtlevel(x ,y+ 1 )> ret )//如果返回等级大于原有收益
        {

            ret = rtlevel(x,y + 1);
            airow =m_ChessPieces[tmpid].m_nRow;
            aicol =m_ChessPieces[tmpid].m_nCol;
        }
    }
    return ret;
}
//函数 通过坐标，返回该点等级，空为负一，翻面或同族为-2
int ChessBoard::rtlevel(int x,int y)
{
    int ret = -1;
    int camp =aipiece;
    int i = 0;
    for(i = 0; i <= 31; i++)
    {
        if( (m_ChessPieces[i].m_bon == 0|| m_ChessPieces[i].m_bRed == camp) && m_ChessPieces[i].m_nRow == x && m_ChessPieces[i].m_nCol == y)
            return -2;
        if( m_ChessPieces[i].m_bon == 1 && m_ChessPieces[i].m_nRow == x && m_ChessPieces[i].m_nCol == y && m_ChessPieces[i].m_bDead == false)
            break;

    }

    if(0<=i && i<32 && (canMove(aipieceid, i, m_ChessPieces[i].m_nRow, m_ChessPieces[i].m_nCol) ) )//
    {     
        ret = m_ChessPieces[i].m_level;
        tmpid = i;
    }

    return ret;
}

void ChessBoard::onrStateChanged(int state)
{
    if (state == Qt::Checked) // "选中"
    {
       //ui->label_3->setText("Checked");
        aired = 1;
    }
    else // 未选中 - Qt::Unchecked
    {
        aired = 0;
    }
}
void ChessBoard::onbStateChanged(int state)
{
    if (state == Qt::Checked) // "选中"
    {
       aiblack =1;
    }
    else // 未选中 - Qt::Unchecked
    {
        aiblack = 0;
    }
}
