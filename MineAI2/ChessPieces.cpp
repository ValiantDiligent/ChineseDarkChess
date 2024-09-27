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
#include "ChessPieces.h"

//1、定义结构体tPOS
struct POS
{
    ChessPieces::m_emTYPE t_emType;
};

//定义基础的16棋子[预定作为上方使用，黑棋使用]
POS tPos[16]= {
              { ChessPieces::CHE},
              { ChessPieces::MA},
              {ChessPieces::XIANG},
              { ChessPieces::SHI},
              { ChessPieces::JIANG},
              {ChessPieces::SHI},
              {ChessPieces::XIANG},
              { ChessPieces::MA},
              {ChessPieces::CHE},

              {ChessPieces::PAO},
              {ChessPieces::PAO},
              { ChessPieces::BING},
              { ChessPieces::BING},
              {ChessPieces::BING},
              {ChessPieces::BING},
              {ChessPieces::BING}
              };

int mapID[33]={0};
ChessPieces::ChessPieces()
{

}

ChessPieces::~ChessPieces()
{

}
void ChessPieces::getmap()
{
    int arrID[33] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

    for (int i = 31;i>0 ; i--)
       {
           int pos = rand() % i;
           mapID[i] = arrID[pos];
           for (int j = pos;j<32;j++)
           {
               arrID[j] = arrID[j+1];
           }
       }
    mapID[0] = arrID[0];
}

//初始化  对每一个棋子进行检验判断而后赋相应的值
void ChessPieces::init(int id)
{

    if(id <16)
    {
    m_nRow = mapID[id]/8 ;// 可以计算
    m_nCol = mapID[id]%8;// 可以计算
        m_emType = tPos[id].t_emType; //赋值类型
        m_bRed = false;
        m_level = trans(); //类型转等级
        m_bon = 0;//默认反面
    }
    else
    {
        m_nRow = mapID[id]/8 ;// 可以计算
        m_nCol = mapID[id]%8;// 可以计算
        m_emType = tPos[id-16].t_emType;
        m_bRed = true;
        m_level = trans();
        m_bon = 0;//默认反面
    }

    m_bDead = false;
}


int ChessPieces::trans() //转换等级
{
    switch(m_emType)
    {
    case CHE:
        return 3;
    case MA:
        return 2;
    case PAO:
        return 1;
    case BING:
        return 0;
    case JIANG:
        return 6;
    case SHI:
        return 5;
    case XIANG:
        return 4;
    default:
        return -1;
    }

    return -1;
}

QString ChessPieces::getnName()
{
    //enum m_emTYPE{JIANG, SHI, XIANG, MA, CHE, PAO, BING};
    if(m_bRed)
    {
        switch(m_emType)
        {
        case CHE:
            return "车";
        case MA:
            return "马";
        case PAO:
            return "炮";
        case BING:
            return "兵";
        case JIANG:
            return "帅";
        case SHI:
            return "仕";
        case XIANG:
            return "相";
        default:
            return "显示出错";
        }
    }
    else
    {
        switch(m_emType)
        {
        case CHE:
            return "车";
        case MA:
            return "马";
        case PAO:
            return "炮";
        case BING:
            return "卒";
        case JIANG:
            return "将";
        case SHI:
            return "士";
        case XIANG:
            return "象";
        default:
            return "显示出错";
    }
        }


    return "ERROR";

}
