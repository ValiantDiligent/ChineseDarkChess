
#include "NetworkGame.h"
#include <QDebug>

NetworkGame::NetworkGame(bool isServer)
{
    m_tcpServer = NULL;
    m_tcpSocket = NULL;

    if(isServer) //作为服务器端
    {
        m_bIsTcpServer = true;
        isclick = true;
        m_bIsRed = true;
        m_tcpServer = new QTcpServer(this);
        m_tcpServer->listen(QHostAddress::Any, 9999);
       bool b = connect(m_tcpServer, SIGNAL(newConnection()),
                        this, SLOT(slotNewConnection()));//监听新链接
       qDebug()<<"服务端信号槽："<<b;
    }
    else   //作为客户端
    {
        m_bIsTcpServer = false;
        m_bIsRed = true;
        isclick =false;
        m_tcpSocket = new QTcpSocket(this);
        m_tcpSocket->connectToHost(QHostAddress("127.0.0.1"), 9999);
        bool b =   connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(slotRecv()));
        qDebug()<<"客户端"<<b;
    }
}

NetworkGame::~NetworkGame()
{

}

void NetworkGame::clickPieces(int checkedID, int &row, int &col)
{
    //不能够替对方选棋和下棋
    if(m_bIsTcpServer) //作为服务器一方  不能替黑棋下棋 服务器红
    {
        if(m_nSelectID == -1 && m_nCheckedID != -1 )//想要选中 /翻棋
        {
            if ((m_ChessPieces[checkedID].m_bon)&& (m_bIsTcpServer != m_ChessPieces[checkedID].m_bRed ))
                return ;
         if(m_bIsRed != m_bIsTcpServer)
             return;

        }

    }
    else  //作为客户端一方  不能替红棋下棋
    {
        //选棋[非下棋]这一步过程，使得其无法选择中红棋
        if(m_nSelectID == -1 && m_nCheckedID != -1)
        {
            if((m_ChessPieces[checkedID].m_bon)&&(m_bIsTcpServer != m_ChessPieces[checkedID].m_bRed ) )
                return ;
            if(m_bIsRed != m_bIsTcpServer)
                return;
        }
    }

    isclick = !isclick;
    isWin();
    ChessBoard::clickPieces(checkedID, row, col);
    char arry[4];
    arry[0] = 2;
    arry[1] = checkedID;
    arry[2] = row;
    arry[3] = col;
    m_tcpSocket->write(arry, 4);
}


void NetworkGame::slotNewConnection()
{
    if(m_tcpSocket) //第三人打扰则返回
        return;
    m_tcpSocket = m_tcpServer->nextPendingConnection();
    qDebug()<<"服务端连接到客户端";
//
   char buf[33];
   buf[0]=1;
   for(int i = 0;i <32;i++)
   {
       buf[i+1]=mapID[i];
   }
   copymap(mapID);
   m_tcpSocket->write(buf,33);
   //调试用
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(slotRecv()));
}

void NetworkGame::slotRecv()
{
    qDebug()<<"客户端收到";
    QByteArray arry = m_tcpSocket->readAll(); //字节数组
    char cmd = arry[0];
    if(cmd == 1)
    {
           //棋盘重新绘制

                int A[32]={0};
                for(int i=0;i<32;i++)
                {
                   A[i]=arry[i+1];
                }
                copymap(A);
                //
                 qDebug()<<"重绘，修改后的坐标的坐标是：";
                 for(int j=0;j<32;j++)
                 {
                     qDebug() <<m_ChessPieces[j].m_nRow<<m_ChessPieces[j].m_nCol;
                 }


    }
    if(cmd==2){
        //点击信息
        int nCheckedID = arry[1];
        int nRow = arry[2];
        int nCol = arry[3];
        ChessBoard::clickPieces(nCheckedID, nRow, nCol);
      //   qDebug()<<"点击成功id:"<<nCheckedID<<"  row:"<<nRow<<"  col"<<nCol;
    }


}

/*
 * 给对方发生数据
 * //服务端发出，客户端接受
 * 1、棋盘同步 第一个字节为3
 * 2、红黑 第一个字节为一 第二个字节为0/1 零接收黑，一接收方红
 * //
 * 3、点击信息 第一字节固定为二 id row col
*/
