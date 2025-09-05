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
    CImage m_image;//����ͼ��
    CRect m_rect;//ͼ��λ��
    BOOL   m_isFaceUp;//�Ǳ��滹������
    BOOL m_bIsSelected;
    
    int m_suit;//��ɫ
    int m_rank;//�˿�����ֵ
    
    void Draw(CDC* pDC);//���Լ�������
    BOOL HitTest(CPoint point);//�ж��Ƿ�����������
    static CImage s_cardBackImage;//����ͼ��
    static void LoadBackImage();//���ر���ͼƬ

    friend bool operator< (const CCard& pCard1, const CCard& pCard2);
};

