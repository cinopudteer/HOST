#pragma once

#define SUIT_DIAMOND 0
#define SUIT_CLUB    1
#define SUIT_HEART   2
#define SUIT_SPADE   3
#define SUIT_JOKER   4

#define SUIT_BACK    -1


#define RANK_3  0
#define RANK_4  1 
#define RANK_5  2 
#define RANK_6  3 
#define RANK_7  4 
#define RANK_8  5 
#define RANK_9  6 
#define RANK_10 7 
#define RANK_J  8
#define RANK_Q  9
#define RANK_K  10
#define RANK_A  11
#define Rank_2  12
#define RANK_SK 13
#define RANK_BK 14
#define RANK_BACK -1
#include <afx.h>
class CCard :
    public CObject
{
public:
    CCard(int suit,int rank);
    ~CCard();
    CImage m_image;//正面图像
    CRect m_rect;//图像位置
    BOOL   m_isFaceUp;//是背面还是正面
    BOOL m_bIsSelected;
    
    int m_suit;//花色
    int m_rank;//扑克牌数值
    
    void Draw(CDC* pDC);//将自己画出来
    BOOL HitTest(CPoint point);//判断是否点击了这张牌
    static CImage s_cardBackImage;//背面图像
    static void LoadBackImage();//加载背面图片

    friend bool operator< (const CCard& pCard1, const CCard& pCard2);
};

