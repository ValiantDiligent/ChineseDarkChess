/*
 * Copyright (C)  2019  与子偕臧.  All rights reserved.
 *
 * Author:  与子偕臧 xmulitech@gmail.com
 *
 * github:  https://github.com/xmuli
 * blogs:   https://xmuli.tech
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://touwoyimuli.github.io/>.
 */
#ifndef CHESSPIECES_H
#define CHESSPIECES_H

#include <QString>
//#include <cstdlib>
//#include <QRect>
//#include <QPainter>
extern int mapID[];
class ChessPieces
{
public:
    ChessPieces();

    ~ChessPieces();
   void getmap();
   void copymap();
    void init(int id);  //初始化
    //void rotate();   //轮转函数
    QString getnName();    //棋子对应的汉字
    int trans();
    enum m_emTYPE{JIANG, SHI, XIANG, MA, CHE, PAO, BING};

    int  m_nRow;   //行
    int  m_nCol;   //列
    int  m_nID;    //ID号
    bool m_bDead;  //死亡状态
    bool m_bRed;   //是否是红方
    bool m_bon;    //是否正面
    int m_level; //棋子等级
    m_emTYPE m_emType;  //具体哪一个棋子
};

#endif // CHESSPIECES_H
